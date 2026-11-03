/*
 * A136191  Primes p such that 2p-3 and 2p+3 are both prime (A092110), with
 *          last decimal being 3.
 * https://oeis.org/A136191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A136191: primes p for which 2p-3 and 2p+3 are also prime,
   and p ends with digit 3 (OEIS offset 1).  This function
   generates the sequence lazily and memoises results. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined below offset */

    /* static memoisation arrays */
    static seq_t *seq      = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;
    static seq_t last_p    = 0;          /* last prime found in sequence */

    /* initialise on first call */
    if (!seq) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        if (!seq) exit(EXIT_FAILURE);
        seq[0] = 13;                     /* first term of the sequence */
        seq_len = 1;
        last_p = 13;
    }

    /* extend sequence until it contains at least n terms */
    while (seq_len < n) {
        /* next candidate ending with 3 */
        seq_t cand = last_p + (seq_t)10;
        while (!is_prime_big(cand))
            cand += (seq_t)10;            /* skip non‑primes */

        /* check the twin‑prime‑like conditions */
        seq_t two = cand + cand;          /* 2 * cand */
        if (!is_prime_big(two - (seq_t)3))
            { last_p = cand; continue; }
        if (!is_prime_big(two + (seq_t)3))
            { last_p = cand; continue; }

        /* accept the prime */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = mem_resize(seq_t, seq, seq_cap);
            if (!seq) exit(EXIT_FAILURE);
        }
        seq[seq_len++] = cand;
        last_p = cand;
    }

    return seq[n-1];
}

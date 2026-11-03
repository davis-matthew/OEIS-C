/*
 * A118939  Primes p such that (p^2+3)/4 is prime.
 * https://oeis.org/A118939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 37, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code finds a prime p such that (p^2+3)/4 is also prime.
   It enumerates primes in increasing order, tests the condition,
   and stores successful primes in a static cache so that subsequent
   calls are O(1) after the cache is built. */

#include <stdbool.h>
#include <stdlib.h>
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
    /* Cache of discovered sequence terms. */
    static seq_t *seq      = NULL;
    static size_t seq_cnt  = 0;
    static size_t seq_cap  = 0;

    /* Next integer to test for primality. */
    static idx_t next_candidate = 2;

    if (n == 0) return (seq_t)0;    /* 1‑based sequence; unused value */

    /* Expand the cache until it contains at least n terms. */
    while (seq_cnt < (size_t)n) {
        /* Find the next prime candidate. */
        seq_t cand = (seq_t)next_candidate;
        while (!is_prime_big(cand)) {
            ++next_candidate;
            cand = (seq_t)next_candidate;
        }
        seq_t p = cand;

        /* Compute t = (p^2+3)/4 using 128‑bit to avoid overflow. */
        unsigned __int128 prod = (unsigned __int128)p * (unsigned __int128)p;
        seq_t t = (seq_t)((prod + 3ULL) / 4ULL);

        if (is_prime_big(t)) {
            /* Grow the cache if necessary. */
            if (seq_cap <= seq_cnt) {
                size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
                seq_t *tmp = mem_resize(seq_t, seq, new_cap);
                if (!tmp) {
                    mem_free(seq);
                    seq = NULL;
                    seq_cnt = 0;
                    seq_cap = 0;
                    return (seq_t)0;
                }
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_cnt++] = p;
        }

        /* Advance to the next integer for the next iteration. */
        ++next_candidate;
    }

    return seq[n - 1];
}

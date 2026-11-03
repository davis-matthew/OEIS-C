/*
 * A155007  Primes p such that (p-3)*(p+3)-+3*p are primes.
 * https://oeis.org/A155007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that (p-3)*(p+3)-3p and (p-3)*(p+3)+3p are primes
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *seq = NULL;       /* stored Sequence values */
static size_t seq_len = 0;      /* current number of stored terms */
static size_t seq_cap = 0;      /* capacity of the seq array */
static seq_t next_candidate = 2;/* next integer to test for being a prime */

/* Ensure the sequence contains at least n elements (1‑based) */
static void generate_upto(idx_t n)
{
    if (seq_len >= (size_t)n) return;          /* already have enough */

    /* initialize storage if first call */
    if (!seq) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
    }

    seq_t p = next_candidate; /* start from the last used candidate */

    while (seq_len < (size_t)n) {
        /* Find next prime candidate */
        while (!is_prime_big(p)) ++p;

        /* p is prime, compute the two expressions */
        seq_t p2 = p * p;
        seq_t a  = p2 + 3 * p - 9; /* (p-3)*(p+3)-3p = p^2-3p-9 */
        seq_t b  = p2 - 3 * p - 9; /* (p-3)*(p+3)+3p = p^2+3p-9 */

        if (is_prime_big(a) && is_prime_big(b)) {
            /* store p in the sequence */
            if (seq_len >= seq_cap) {
                seq_cap <<= 1;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = p;
        }
        ++p; /* move to next integer */
    }

    next_candidate = p; /* remember where we left off */
}

/* External linkage : returns the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n==0, return 0 */
    generate_upto(n);
    return seq[n - 1];
}

/*
 * A200652  a(n) = least k with -3<k<2*n such that n*(n+1)+k and n*(n+1)+k+2
 *          are twin primes, or 0 if no such k exists.
 * https://oeis.org/A200652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k with -3<k<2*n such that n*(n+1)+k and n*(n+1)+k+2
 *            are twin primes, or 0 if no such k exists
 * Range:     i8 n <= 10, i16 n <= 941, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9829 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* for bool type */
#include <stdint.h>
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
    /* n is 1‑based; the offset is 1. If n < 1 we return 0 (undefined but harmless). */
    if (n == 0) return 0;            /* guard for accidental 0 index */

    /* Compute n*(n+1) using seq_t arithmetic to avoid overflow. */
    seq_t base = (seq_t)n * ((seq_t)n + 1);

    /* Search for the smallest k in the allowed range. */
    for (seq_t k = -2; k < (seq_t)(2 * n); ++k) {
        seq_t p1 = base + k;
        seq_t p2 = p1 + 2;
        if (is_prime_big(p1) && is_prime_big(p2))
            return k;
    }
    return 0;   /* no suitable k found */
}

/*
 * A200653  a(n) = greatest k with -3<k<2*n such that n*(n+1)+k and
 *          n*(n+1)+k+2 are twin primes.
 * https://oeis.org/A200653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = greatest k with -3 < k < 2*n such that * n*(n+1)+k and
 *            n*(n+1)+k+2 are twin primes
 * Range:     i8 n <= 10, i16 n <= 409, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9351 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation tests each candidate k in descending order
 * and uses the utility function is_prime_big() to check primality. */

#include <stdbool.h>
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
    /* main base = n*(n+1) */
    seq_t base = (seq_t)n * ((seq_t)n + 1);

    /* Search from k = 2*n-1 down to k = -2 (since k > -3). */
    for (int64_t k = (int64_t)2 * (int64_t)n - 1; k >= -2; --k)
    {
        seq_t N = base + (seq_t)k;
        /* Twin primes must be at least 2; for n>=1 this is always true. */
        if (is_prime_big(N) && is_prime_big(N + 2))
            return (seq_t)k;
    }
    /* If no suitable k is found, return 0. */
    return (seq_t)0;
}

/*
 * A074846  a(n) is the smallest k > n such that n*k+1 is prime.
 * https://oeis.org/A074846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > n such that n*k + 1 is prime
 * Range:     u8 n <= 12, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

/* Computes a(n) for OEIS sequence A074846.
 *
 *   a(n) = min{ k > n : n*k + 1 is prime }.
 *
 * The function uses trial increments starting at k = n+1 and tests
 * n*k+1 for primality with the provided is_prime_big() routine.
 * For 64‑bit builds the product is computed in 128‑bit to avoid
 * overflow before being cast to seq_t.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to seq_t for multiplication */
    for (idx_t k = n + 1; ; ++k)
    {
        /* compute n*k + 1 using 128‑bit intermediate to avoid overflow
           for 64‑bit seq_t builds. */
        __uint128_t tmp = (__uint128_t)sn * k + 1;
        seq_t prod = (seq_t)tmp;
        if (is_prime_big(prod))
            return (seq_t)k;     /* returned k fits in seq_t */
    }
}

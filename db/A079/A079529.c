/*
 * A079529  a(n) = sigma(n) - ceiling(n + sqrt n) as n runs through the
 *          composite numbers A002808.
 * https://oeis.org/A079529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(c_n) - ceil(c_n + sqrt(c_n)), where c_n is the n‑th
 *            composite number
 * Range:     u8 n <= 44, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-01)
 * Helpers:   ipow_i32, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Find the n-th composite number c_n (c_1 = 4). */
    idx_t count = 0;
    idx_t m = 4;                /* first composite */
    while (count < n)
    {
        if (!is_prime(m))      /* composite */
        {
            ++count;
            if (count == n)
                break;
        }
        ++m;
    }

    /* Compute sigma(c_n). */
    seq_t sigma = sigma_of(m);

    /* Compute ceil(c_n + sqrt(c_n)). */
    seq_t sf   = floor_sqrt((seq_t)m);          /* floor sqrt(c_n)      */
    seq_t ceil_val;
    if (sf * sf == (seq_t)m)                    /* perfect square?      */
        ceil_val = (seq_t)m + sf;
    else
        ceil_val = (seq_t)m + sf + (seq_t)1;    /* ceil value         */

    return sigma - ceil_val;
}

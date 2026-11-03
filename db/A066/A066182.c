/*
 * A066182  Permutation of the integers with cycle form {1}, {3, 2}, {6, 5,
 *          4}, {10, 9, 8, 7}, ...
 * https://oeis.org/A066182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = n-1 + binom(round(sqrt(2*n))+1,2) -
 *            binom(round(sqrt(2*(n-1)))+1,2)
 * Range:     u8 n <= 253, u16 n <= 45150, u32 n <= 45150, u64 n <= 45150,
 *            u128 n <= 45150, big any n
 * Validated: standard, pass, 88 terms through n = 9508 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation avoids floating point and integer overflow by using 128‑bit arithmetic. */

#include <stdint.h>

/* Compute binomial(k,2) for unsigned values using 128‑bit intermediate. */
static seq_t binom2(seq_t k)
{
    /* binom(k,2) = k*(k-1)/2 */
    return (seq_t)(((__uint128_t)k * (k - 1)) / 2);
}

/* Compute floor(sqrt(2*n)) by binary search.  Works for any idx_t up to 2^64-1. */
static idx_t floor_sqrt_of_2n(idx_t n)
{
    /* Upper bound for sqrt(2*n) is < 2^33 for 64‑bit idx_t. */
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << (sizeof(idx_t)*8/2 + 1)) - 1;   /* sufficiently large */

    __uint128_t target = (__uint128_t)n * 2;   /* 2*n as 128‑bit to avoid overflow */

    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 <= target)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Round sqrt(2*n) to nearest integer.  Returns the integer k such that
   k is nearest to sqrt(2*n) (ties rounded up). */
static idx_t round_sqrt_of_2n(idx_t n)
{
    idx_t s   = floor_sqrt_of_2n(n);
    __uint128_t left  = (__uint128_t)n * 2 - (__uint128_t)s * s;
    __uint128_t right = (__uint128_t)(s + 1) * (s + 1) - (__uint128_t)n * 2;
    return (left <= right) ? s : s + 1;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    idx_t t = round_sqrt_of_2n(n);           /* A002024(n)   */
    idx_t u = round_sqrt_of_2n(n - 1);       /* A002024(n-1) */

    /* binom(round(sqrt(2*n))+1,2) = binom(t+1,2) */
    seq_t binot = binom2((seq_t)(t + 1));
    seq_t binou = binom2((seq_t)(u + 1));

    /* a(n) = n-1 + binot - binou  (all operations in seq_t) */
    return (seq_t)n - 1 + binot - binou;
}

/*
 * A130209  Diagonalized matrix of d(n), A000005, number of divisors of n.
 * https://oeis.org/A130209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = d(k) if n = k*(k+1)/2 (k ≥ 1), a(n) = 0 otherwise. Here
 *            d(k) is the number of positive divisors of k
 * Range:     u8 n <= 9734, u16 n <= 9734, u32 n <= 9734, u64 n <= 9734,
 *            u128 n <= 9734, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function divisor_count_u64 is provided by the build.
   It returns the divisor count of a 64‑bit unsigned integer. */
#include "../utils/divisor_count_u64.h"

/* Integer floor square root for a 64‑bit unsigned integer.
   (No external library needed – a simple binary search.) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (n >> 1) + 1;          /* hi ≥ sqrt(n) for n > 1 */
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        /* Use 128‑bit to avoid overflow while comparing mid*mid <= n. */
        if ((__uint128_t)mid * mid <= (__uint128_t)n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;        /* floor sqrt(n) */
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* indices start at 1 */

    /* 8*n + 1  is odd; if it is a perfect square (s^2),
       then n is a triangular number with index k = (s-1)/2.   */
    idx_t d   = 8 * n + 1;
    idx_t s   = isqrt_u64(d);

    if ((__uint128_t)s * s != (__uint128_t)d) return 0;   /* not triangular */
    if ((s & 1) == 0) return 0;                            /* s must be odd */
    idx_t k = (s - 1) >> 1;                                 /* k ≥ 1 for n ≥ 1 */

    /* Double‑check for safety (trivial for the range but keeps the definition clean). */
    if ((__uint128_t)k * (k + 1) / 2 != n) return 0;

    /* Return the number of divisors of k, cast to seq_t. */
    return (seq_t)divisor_count_u64((uint64_t)k);
}

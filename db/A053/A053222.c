/*
 * A053222  First differences of sigma(n).
 * https://oeis.org/A053222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = σ(n+1) – σ(n) (first differences of the sum‑of‑divisors
 *            function)
 * Range:     i8 n <= 82, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   sum_of_divisors_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_of_divisors_uint64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 the values are
       irrelevant but we return 0 to keep the function well‑defined. */
    if (n == 0)
        return 0;

    uint64_t n1 = (uint64_t)n;
    uint64_t n2 = n1 + 1U;
    __int128 s1 = sum_of_divisors_uint64(n1);
    __int128 s2 = sum_of_divisors_uint64(n2);
    return (seq_t)(s2 - s1);
}

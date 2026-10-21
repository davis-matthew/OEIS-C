/*
 * A093001  Least k such that Sum_{r=n+1..k} r is greater than or equal to the
 *          sum of the first n positive integers (i.e., the n-th triangular
 *          number, A000217(n)). Or, least k such that (sum of first n
 *          positive integers) <= (sum of numbers from n+1 up to k).
 * https://oeis.org/A093001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil((-1 + sqrt(8*n^2 + 8*n + 1)) / 2) Computation by
 *            accumulating consecutive sums from n+1 upward
 * Range:     u8 n <= 15, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


seq_t a(idx_t n)
{
    /* Convert the index to the precision of seq_t. */
    seq_t N = (seq_t)n;

    /* The triangular number T_n = n(n+1)/2. */
    seq_t target = N * (N + (seq_t)1);
    target = target / (seq_t)2;

    /* Start adding numbers from n+1 until the sum reaches or exceeds T_n. */
    seq_t k   = N + (seq_t)1;
    seq_t sum = (seq_t)0;

    while (sum < target) {
        sum += k;
        k++;
    }

    /* k is the first number that caused sum >= target; the required
       minimal k is k-1. */
    return k - (seq_t)1;
}

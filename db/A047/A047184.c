/*
 * A047184  Number of nonempty subsets of {1,2,...,n} in which exactly 2/3 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1} C(floor((n-2)/2), 2k) * C(n - floor((n-2)/2),
 *            k)
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Where the sum runs over all k such that 2k <= floor((n-2)/2) and k <= n - floor((n-2)/2). */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* For n < 3 the low range is too small to allow a non‑empty subset.  */
    if (n < 3) return 0;

    /* low contains the number of elements <= (n-2)/2 (integer floor).  */
    idx_t low  = (n - 2) / 2;
    idx_t high = n - low;

    /* k must satisfy 2k <= low and k <= high.  */
    idx_t max_k = low / 2;
    if (max_k > high) max_k = high;

    seq_t sum = 0;
    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t low_part  = binom(low, 2 * k);
        seq_t high_part = binom(high, k);
        sum += low_part * high_part;
    }
    return sum;
}

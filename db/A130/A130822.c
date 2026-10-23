/*
 * A130822  Two 1's, one 2, four 3's, three 4's ...
 * https://oeis.org/A130822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer k such that * the cumulative
 *            multiplicities up to k reach or exceed n. * For each integer i ≥
 *            1: * m(i) = i + 1 if i is odd, * m(i) = i – 1 if i is even. *
 *            Thus the partial sums are: * P(2t) = 2t² + t * P(2t+1) = 2t² +
 *            3t + 2 * where P(k) = Σ_{i=1}^{k} m(i). * a(n) =...
 * Range:     u8 n <= 44, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   partial_sum, partial_sum_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/partial_sum.h"
#include "../utils/partial_sum_even.h"

seq_t a(idx_t n)
{
    /* binary search for minimal k such that partial_sum(k) >= n. */
    idx_t low = 1, high = n;          /* high is safe because P(n) >= n for n>=1 */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (partial_sum(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}

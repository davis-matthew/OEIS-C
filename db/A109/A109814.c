/*
 * A109814  a(n) is the largest k such that n can be written as sum of k
 *          consecutive positive integers.
 * https://oeis.org/A109814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max k such that there exists integer m ≥ 1 with n = m +
 *            (m+1) + … + (m+k-1) (i.e. 2n = k·(2m + k – 1) )
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* 2n as seq_t */
    seq_t two_n = (seq_t)n + (seq_t)n;

    /* Upper bound for k: k^2 ≤ 2n  =>  k ≤ sqrt(2n) */
    idx_t kmax = (idx_t)sqrtl((long double)n * 2.0L);

    for (idx_t k = kmax; k > 0; --k)
    {
        seq_t k_seq = (seq_t)k;
        if ((two_n % k_seq) != 0)
            continue;

        seq_t diff = (two_n / k_seq) - k_seq + 1;
        if (diff > 0 && (diff & 1) == 0)
            return (seq_t)k;
    }

    return (seq_t)1;   /* only possible for n = 2^t */
}

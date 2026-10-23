/*
 * A104513  The number of consecutive integers > 1 beginning with A104512(n),
 *          the sum of which equals n, or 0 if impossible.
 * https://oeis.org/A104513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = maximum k ≥ 2 such that n = k·a + k(k−1)/2 with a ≥ 1, *
 *            else 0. This k is the length of the longest run of consecutive
 *            positive * integers that sums to n
 * Range:     u8 n <= 9927, u16 n <= 9927, u32 n <= 9927, u64 n <= 9927,
 *            u128 n <= 9927, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* longest length found so far */
    seq_t best_k = 0;

    /* iterate k from 2 upwards until k(k+1)/2 > n */
    for (idx_t k = 2; ; ++k) {
        /* check the minimal possible sum k(k+1)/2  */
        __int128 min_sum = (__int128)k * (k + 1) / 2;
        if (min_sum > (__int128)n)
            break;

        /* remaining part to be covered by k·a  */
        __int128 rem = (__int128)n - (__int128)k * (k - 1) / 2;
        if (rem <= 0)
            continue;          /* should not occur for small k */

        /* rem must be divisible by k to get integer a */
        if (rem % k == 0) {
            seq_t a_start = (seq_t)(rem / k);
            if (a_start >= 1 && k > best_k)
                best_k = (seq_t)k;
        }
    }
    return best_k;
}

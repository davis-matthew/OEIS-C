/*
 * A060065  Smallest mode of the sequence { C(n-k,k), k=0..n/2 }.
 * https://oeis.org/A060065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k (0 ≤ k ≤ ⌊n/2⌋) for which the binomial
 *            coefficient * C(n−k, k) attains its maximum value in the
 *            sequence { C(n−k, k) : k=0 … ⌊n/2⌋ }. * The sequence is
 *            unimodal; comparing successive terms yields the rule * C(n−k−1,
 *            k+1) > C(n−k, k) ⇔ (n−2k)(n−2k−1) > (k+1)(n−k)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
seq_t a(idx_t n)
{
    idx_t half = n / 2;        /* upper bound for k */
    idx_t k = 0;
    /* iterate while the next term is larger than the current one */
    while (k < half) {
        unsigned __int128 left  = (unsigned __int128)(n - 2 * k)
                                * (unsigned __int128)(n - 2 * k - 1);
        unsigned __int128 right = (unsigned __int128)(k + 1)
                                * (unsigned __int128)(n - k);
        if (left > right) {    /* still increasing */
            ++k;
        } else {
            break;             /* peak reached or plateau */
        }
    }
    return (seq_t)k;           /* smallest k attaining the maximal value */
}

/*
 * A124443  a(1)=1, a(n) = LCM of the integers, from 1 to n/2, which are
 *          coprime to n.
 * https://oeis.org/A124443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM{ k : 1 <= k <= n/2 and gcd(k,n) = 1 }
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* n is at least 1 */
    seq_t lim = (seq_t)(n / 2);
    seq_t result = (seq_t)1;
    seq_t n_seq  = (seq_t)n;

    for (idx_t i = 1; i <= lim; ++i) {
        seq_t i_seq = (seq_t)i;
        if (seq_gcd(i_seq, n_seq) == (seq_t)1) {
            seq_t g = seq_gcd(result, i_seq);
            result = (result / g) * i_seq;
        }
    }
    return result;
}

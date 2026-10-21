/*
 * A214745  Least m>0 such that n-m divides 2*n-1+m.
 * https://oeis.org/A214745
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - max{ d | d < n and d divides (3*n - 1) }
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t X = (seq_t)n * 3 - 1;
    seq_t best = 1;
    for (idx_t i = 1; (seq_t)i * (seq_t)i <= X; ++i) {
        seq_t i_seq = (seq_t)i;
        if (X % i_seq == 0) {
            seq_t d1 = i_seq;
            if (d1 < (seq_t)n && d1 > best)
                best = d1;
            seq_t d2 = X / i_seq;
            if (d2 < (seq_t)n && d2 > best)
                best = d2;
        }
    }
    return (seq_t)n - best;
}

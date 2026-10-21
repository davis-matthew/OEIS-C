/*
 * A053472  a(n) is the cototient of n (A051953) iterated 4 times.
 * https://oeis.org/A053472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = apply f(x) = x - phi(x) four times, with f(0)=0
 * Range:     u8 n <= 461, u16 n <= 9751, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include <stdbool.h>

seq_t a(idx_t n) {
    /* cast the index to the output type */
    seq_t m = (seq_t)n;
    /* iterate up to four times, stopping if the value becomes 0 */
    for (int i = 0; i < 4 && m > 0; ++i) {
        m = m - calc_phi(m);
    }
    return m;
}

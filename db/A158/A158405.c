/*
 * A158405  Triangle T(n,m) = 1+2*m of odd numbers read along rows, 0<=m<n.
 * https://oeis.org/A158405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*i-1, where i = n - t(t+1)/2, t = floor( (-1 +
 *            sqrt(8*n-7)) / 2 )
 * Range:     u8 n <= 8384, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   _triangle_probe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_triangle_probe.h"

seq_t a(idx_t n) {
    idx_t t  = _triangle_probe(n);
    idx_t tri = (idx_t)((unsigned __int128)t * (t + 1) / 2);
    idx_t i   = n - tri;
    seq_t res = ((seq_t)2 * (seq_t)i) - (seq_t)1;
    return res;
}

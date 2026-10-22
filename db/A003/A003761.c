/*
 * A003761  Number of spanning trees in D_4 X P_n.
 * https://oeis.org/A003761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * U(n-1,3/2) * U(n-1,5/2) * U(n-1,3), where D_4 is the
 *            paw graph (Laplacian eigenvalues 0,1,3,4) and U is the Chebyshev
 *            polynomial of the second kind; equivalently s_x(0) = 1, s_x(1) =
 *            x, s_x(k) = x*s_x(k-1) - s_x(k-2) for x = 3, 5, 6, and a(n) = 3
 *            * s_3(n-1) * s_5(n-1) * s_6(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   cheb_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cheb_u.h"

seq_t a(idx_t n) {
    idx_t k = n - 1;
    seq_t r = (seq_t)3;
    r = r * cheb_u((seq_t)3, k);
    r = r * cheb_u((seq_t)5, k);
    r = r * cheb_u((seq_t)6, k);
    return r;
}

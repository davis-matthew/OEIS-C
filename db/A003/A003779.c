/*
 * A003779  Number of spanning trees in P_5 x P_n.
 * https://oeis.org/A003779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{j=1..4} U_{n-1}(2 - cos(j*Pi/5)), U = Chebyshev
 *            polynomial of the 2nd kind, i.e. a(n) = N(U_{n-1}((4-phi)/2)) *
 *            N(U_{n-1}((5-phi)/2)) with N the norm of Z[phi], phi =
 *            (1+sqrt(5))/2; each factor follows U_k = t*U_{k-1} - U_{k-2},
 *            U_{-1} = 0, U_0 = 1, computed in Z[psi], psi = 1 - phi
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   norm_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/norm_factor.h"

seq_t a(idx_t n) {
    return norm_factor(n, 3) * norm_factor(n, 4);
}

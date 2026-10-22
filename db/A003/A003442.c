/*
 * A003442  Number of nonequivalent dissections of an n-gon into (n-3)
 *          polygons by nonintersecting diagonals rooted at a cell up to
 *          rotation.
 * https://oeis.org/A003442
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( (n-3)*C(2n-5,n-4) + [n even]*n*A000108(n/2-1)/4 +
 *            [4|n]*(n/2)*A000108(n/4-1) ) / n (Burnside's lemma over the
 *            rotations: C(2n-5,n-4) dissections into one quadrilateral and
 *            n-4 triangles, each with n-3 cells to choose as the root. Only a
 *            centred quadrilateral can be fixed by a rotation of...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   a3442_bin, a3442_cat, a3442_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3442_bin.h"
#include "../utils/a3442_cat.h"
#include "../utils/a3442_g.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;
    seq_t B = a3442_bin(2 * n - 5, n - 4);
    seq_t q = B / (seq_t)n;
    seq_t r = B % (seq_t)n;
    seq_t X = (seq_t)4 * (seq_t)(n - 3) * r;
    if (n % 2 == 0) X += (seq_t)n * a3442_cat(n / 2 - 1);
    if (n % 4 == 0) X += (seq_t)2 * (seq_t)n * a3442_cat(n / 4 - 1);
    return (seq_t)(n - 3) * q + X / ((seq_t)4 * (seq_t)n);
}

/*
 * A003906  Degrees of irreducible representations of Janko group J3.
 * https://oeis.org/A003906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = degree of the n-th irreducible character of the Janko
 *            group J3, sorted in increasing order (finite sequence of 21
 *            terms; sum of a(n)^2 = |J3| = 50232960). The character degrees
 *            are irregular group-theoretic constants taken from the J3
 *            character table
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t j3_degrees[21] = {
    1u, 85u, 85u, 323u, 323u, 324u, 646u, 646u, 816u, 1140u,
    1215u, 1215u, 1615u, 1920u, 1920u, 1920u, 1938u, 1938u,
    2432u, 2754u, 3078u
};

seq_t a(idx_t n)
{
    seq_t r = 0;
    if (n < 1 || n > 21)
        return r;
    r += (seq_t)j3_degrees[n - 1];
    return r;
}

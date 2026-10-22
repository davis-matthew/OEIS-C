/*
 * A006079  Number of asymmetric planted projective plane trees with n+1
 *          nodes; bracelets (reversible necklaces) with n black beads and n-1
 *          white beads.
 * https://oeis.org/A006079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(1) = a(2) = 1; a(n+1) = (CatalanNumber(n) -
 *            binomial(n,floor(n/2)))/2 for n >= 2, where Catalan(k+1) =
 *            Catalan(k)*(4k+2)/(k+2), Catalan(0) = 1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   a006079_big, a006079_u128, a006079_ugcd, seq_is_fixed_width,
 *            u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a006079_big.h"
#include "../utils/a006079_u128.h"
#include "../utils/a006079_ugcd.h"

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    if (seq_is_fixed_width()) return u128_to_seq(a006079_u128((uint64_t)(n - 1)));
    return a006079_big((uint64_t)(n - 1));
}

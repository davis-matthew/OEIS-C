/*
 * A186153  Rank of n^2 when {(1/8)i^3: i>=1} and {j^2>: j>=1} are jointly
 *          ranked with (1/8)i^3 before j^2 when (1/8)i^3=j^2. Complement of
 *          A186152.
 * https://oeis.org/A186153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊∛(8·n²)⌋
 * Range:     u8 n <= 2, u16 n <= 63, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = rank of n² among the union of {(1/8)i³ : i≥1} and {j² : j≥1} */
/* (with (1/8)i³ before j² when equal). */

#include "../utils/floor_cube_root.h"
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Treat n as seq_t to avoid overflow in intermediate operations. */
    seq_t nn = (seq_t)n;          /* n  */
    seq_t m  = 8 * nn * nn;       /* m = 8·n²  */
    seq_t i_max = floor_cube_root(m); /* i_max = ⌊∛m⌋ */
    return nn + i_max;            /* a(n) = n + i_max */
}

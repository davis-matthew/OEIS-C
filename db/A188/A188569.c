/*
 * A188569  Degree of the n-th partition class polynomial Hpart_n(x).
 * https://oeis.org/A188569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced positive definite binary quadratic
 *            forms of discriminant -(24n-1) (i.e., the class number of the
 *            imaginary quadratic order of discriminant -(24n-1))
 * Range:     u8 n <= 750, u16 n <= 750, u32 n <= 750, u64 n <= 750,
 *            u128 n <= 750, big any n
 * Helpers:   class_number_for_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/class_number_for_d.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* safety for offsets < 1 */

    idx_t d = 24 * n - 1;   /* d = 24n-1, always odd ≡ 3 (mod 4) */
    return class_number_for_d(d);
}

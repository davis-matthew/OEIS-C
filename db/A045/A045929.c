/*
 * A045929  Generalized Connell sequence C_{5,3}.
 * https://oeis.org/A045929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - 4*floor((7 + sqrt(24*n - 23))/6)
 * Range:     u8 n <= 3, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out‑of‑range: OEIS offset is 1 */

    /* 24*n - 23 as a big integer */
    seq_t t = (seq_t)24 * n - 23;

    /* integer square root of t */
    seq_t s = floor_sqrt(t);

    /* floor((7 + s)/6) using truncating integer division */
    seq_t q = (s + 7) / 6;

    /* final result */
    return (seq_t)5 * n - (seq_t)4 * q;
}

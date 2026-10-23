/*
 * A135672  a(n) = floor(n - n^(2/3)).
 * https://oeis.org/A135672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n - n^(2/3)) = n - ceil(n^(2/3))
 * Range:     u8 n <= 300, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   cube_root_floor_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ceil(n^(2/3)) is computed as ceil(cuberoot(n^2))         */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/cube_root_floor_u128.h"

seq_t a(idx_t n)
{
    /* n is of type idx_t (unsigned: uint64_t) */
    if (n == 0) /* out of range, but return 0 safely */
        return (seq_t)0;

    seq_t N = (seq_t)n;                /* Convert index to output type */
    unsigned __int128 n2 = (__int128)n * n; /* n^2 fits in 128 bits */

    uint64_t t = cube_root_floor_u128(n2);   /* floor(cuberoot(n^2)) */

    unsigned __int128 t3 = (__int128)t * t * t;
    bool perfect = (t3 == n2);              /* n^2 is a perfect cube? */
    seq_t ceil_val = (seq_t)(t + (perfect ? 0 : 1));

    return N - ceil_val;
}

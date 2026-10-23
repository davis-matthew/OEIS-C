/*
 * A173677  Number of ways of writing n as a sum of two nonnegative cubes.
 * https://oeis.org/A173677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0..n} c(k) * c(n-k), where c(k)=1 if k is a perfect
 *            cube, otherwise c(k)=0. a(n) counts ordered pairs (x,y) of
 *            non‑negative integers with x³ + y³ = n
 * Range:     u8 n <= 3438, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t max_a = int_cube_root(n);          /* largest a with a³ ≤ n */

    for (idx_t a_val = 0; a_val <= max_a; ++a_val)
    {
        seq_t cube_a = (seq_t)a_val * a_val * a_val;   /* a³ */
        idx_t remainder = n - (idx_t)cube_a;           /* n – a³ */

        idx_t b_val = int_cube_root(remainder);        /* largest b with b³ ≤ remainder */
        seq_t cube_b = (seq_t)b_val * b_val * b_val;   /* b³ */

        if (cube_b == (seq_t)remainder)               /* remainder is a perfect cube */
            count += 1;
    }

    return count;
}

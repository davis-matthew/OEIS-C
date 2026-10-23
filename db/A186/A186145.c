/*
 * A186145  Rank of n^2 when {i^2: i>=1} and {j^3: j>=1} are jointly ranked
 *          with i^2 before j^3 when i^2=j^3. Complement of A186146.
 * https://oeis.org/A186145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((n^2 - 1/2)^(1/3)) Integer equivalent: a(n) = n
 *            + floor(cuberoot(n^2 - 1))
 * Range:     u8 n <= 8, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to sequence type   */
    seq_t n2 = nn * nn;           /* compute n^2                    */
    seq_t m  = n2 - (seq_t)1;     /* m = n^2 - 1 (non‑negative)    */
    seq_t cr = floor_cube_root(m);/* floor(cuberoot(m))             */
    return nn + cr;               /* final result                   */
}

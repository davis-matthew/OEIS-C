/*
 * A169712  The function W_n(8) (see Borwein et al. reference for definition).
 * https://oeis.org/A169712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -33*n + 82*n^2 - 72*n^3 + 24*n^4 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 116, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Treat n as seq_t to avoid intermediate truncation. */
    seq_t s = (seq_t)n;

    seq_t n2 = s * s;          /* n^2 */
    seq_t n3 = n2 * s;         /* n^3 */
    seq_t n4 = n3 * s;         /* n^4 */

    seq_t val = 24 * n4;       /* 24*n^4 */
    val -= 72 * n3;            /* -72*n^3 */
    val += 82 * n2;            /* +82*n^2 */
    val -= 33 * s;             /* -33*n */

    return val;
}

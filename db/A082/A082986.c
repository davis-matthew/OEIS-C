/*
 * A082986  Largest x such that 1/x + 1/y + 1/z = 1/n.
 * https://oeis.org/A082986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n^4 + 2*n^3 + 2*n^2 + n, n >= 1 (Largest x such that 1/x
 *            + 1/y + 1/z = 1/n.)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
/* idx_t and seq_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t once.  All arithmetic below
       is performed in seq_t, so it works for 8‑, 16‑, 32‑, 64‑, 128‑bit
       or arbitrary‑precision builds. */
    seq_t x = (seq_t)n;

    seq_t x2 = x * x;          /* n² */
    seq_t x3 = x2 * x;         /* n³ */
    seq_t x4 = x3 * x;         /* n⁴ */

    /* Compute n^4 + 2*n^3 + 2*n^2 + n */
    seq_t res = x4 + (seq_t)2 * x3 + (seq_t)2 * x2 + x;
    return res;
}

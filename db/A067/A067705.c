/*
 * A067705  a(n) = 11*n^2 + 22*n.
 * https://oeis.org/A067705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*n^2 + 22*n
 * Range:     u8 n <= 3, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External linkage as required by the framework */
seq_t a(idx_t n)
{
    /* The formula works for all n ≥ 1.  For n = 0 the result is 0. */
    /* Use a 128‑bit intermediate to avoid overflow on 32‑bit and 64‑bit seq_t */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 val = 11 * nn * nn + 22 * nn;
    return (seq_t)val;
}

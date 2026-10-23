/*
 * A017139  a(n) = (8*n + 6)^3.
 * https://oeis.org/A017139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 6)^3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 202, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote the index to seq_t and compute the base (8*n + 6). */
    seq_t base = (seq_t)n * (seq_t)8 + (seq_t)6;

    /* Compute the cube with intermediate multiplication to keep the
       arithmetic straightforward and to avoid any unnecessary overflows
       for the supported seq_t sizes. */
    seq_t sq = base * base;      /* base² */
    return sq * base;            /* base³ = (8*n + 6)³ */
}

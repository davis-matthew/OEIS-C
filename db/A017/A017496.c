/*
 * A017496  a(n) = (11*n + 8)^12.
 * https://oeis.org/A017496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (11*n + 8).  All operations are performed
       in the seq_t type, which means that for small seq_t sizes
       the computation is automatically modulo 2^width(seq_t). */
    seq_t base = (seq_t)11 * n + (seq_t)8;

    /* Raise the base to the 12th power by repeated multiplication.
       Since seq_t is an unsigned integer type, overflow behaves
       as modulo arithmetic, matching the expected behaviour for
       arbitrary‑precision and 64‑bit builds. */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 12; ++i)
        result *= base;

    return result;
}

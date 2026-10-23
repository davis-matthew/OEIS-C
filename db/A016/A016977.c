/*
 * A016977  a(n) = (6*n + 5)^9.
 * https://oeis.org/A016977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 5)^9
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 22, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (6*n + 5) in the seq_t type */
    seq_t base = (seq_t)6 * (seq_t)n + (seq_t)5;

    /* Raise base to the 9th power by repeated multiplication.
       This works for any seq_t: unsigned integer types or an
       arbitrary‑precision integer class that supports operator*. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < 9; ++i)
        result *= base;

    return result;
}

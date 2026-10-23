/*
 * A016929  a(n) = (6*n + 1)^9.
 * https://oeis.org/A016929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 1)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 22, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (6*n + 1) first.  Cast n to seq_t
       to avoid narrowing on very small seq_t types. */
    seq_t base = ((seq_t)n) * 6 + 1;

    /* Raise the base to the 9th power by repeated multiplication. */
    seq_t result = 1;
    /* Use a small integer loop counter; 9 is a compile‑time constant. */
    for (int i = 0; i < 9; ++i)
        result *= base;

    return result;
}

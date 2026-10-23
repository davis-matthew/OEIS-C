/*
 * A123867  a(n) = n^10 - 1.
 * https://oeis.org/A123867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10 - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type */
    seq_t x = (seq_t)n;

    /* Compute n^10 using repeated multiplication. */
    /* This works for all seq_t types that support multiplication
       (including arbitrary‑precision integers). */
    seq_t result = 1;
    for (int i = 0; i < 10; ++i)
        result *= x;

    /* Subtract 1 to finish the formula */
    return result - (seq_t)1;
}

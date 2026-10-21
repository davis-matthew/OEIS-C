/*
 * A086746  Multiples of 3018.
 * https://oeis.org/A086746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3018*n
 * Range:     u8 none, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the nth term of the sequence A086746,
       which consists of the multiples of 3018. */
    return 3018 * ((seq_t)n);
}

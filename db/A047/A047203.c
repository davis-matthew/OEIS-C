/*
 * A047203  Numbers that are congruent to {0, 2, 3, 4} mod 5.
 * https://oeis.org/A047203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5*n-2)/4)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence A047203: numbers congruent to 0, 2, 3, or 4 modulo 5. */
    /* Offset: 1 – a(1) = 0, a(2) = 2, … */

    if (n == 0)   /* Outside the defined offset; safe default. */
        return 0;

    return (((seq_t)n * 5) - 2) / 4;
}

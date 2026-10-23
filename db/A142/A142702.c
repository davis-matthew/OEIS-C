/*
 * A142702  Period 4: repeat [5, 2, 5, 8].
 * https://oeis.org/A142702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 + 3*cos(n*Pi/2)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic with period 4: [5, 2, 5, 8]. */
seq_t a(idx_t n)
{
    /* For n >= 1 the pattern repeats every 4 terms. */
    /* (n - 1) & 3 gives a value in {0,1,2,3} corresponding to the terms: */
    /* 0 -> 5, 1 -> 2, 2 -> 5, 3 -> 8. */
    switch ((n - 1) & 3)
    {
        case 0: return (seq_t)5;
        case 1: return (seq_t)2;
        case 2: return (seq_t)5;
        case 3: return (seq_t)8;
    }
    /* Unreachable, but provided for completeness. */
    return (seq_t)0;
}

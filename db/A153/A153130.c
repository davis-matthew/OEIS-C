/*
 * A153130  Period 6: repeat [1, 2, 4, 8, 7, 5].
 * https://oeis.org/A153130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [1, 2, 4, 8, 7, 5][n % 6]
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 6) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)4;
        case 3: return (seq_t)8;
        case 4: return (seq_t)7;
        case 5: return (seq_t)5;
        default: return (seq_t)0; /* never reached */
    }
}

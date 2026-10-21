/*
 * A077169  Initial terms of rows of A077168.
 * https://oeis.org/A077169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - n + 10)/2 for n >= 4 Initial values: a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=7 This implements the OEIS A077169 sequence
 * Range:     u8 n <= 16, u16 n <= 361, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)4;
        case 3: return (seq_t)7;
        default:
        {
            seq_t nn = (seq_t)n;                     /* convert index to seq_t */
            seq_t numer = nn * nn - nn + (seq_t)10;   /* n^2 - n + 10   */
            return numer / (seq_t)2;                 /* divide by 2 (even) */
        }
    }
}

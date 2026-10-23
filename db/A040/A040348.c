/*
 * A040348  Continued fraction for sqrt(368).
 * https://oeis.org/A040348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=19; for n>0 repeat the 4-term cycle 5,2,5,38
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is: a(n)= (n%4==1 || n%4==3 ? 5 : (n%4==2 ? 2 : 38)) for n>0. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)19;
    }
    switch (n % 4) {
        case 1:
        case 3:
            return (seq_t)5;
        case 2:
            return (seq_t)2;
        default: /* n % 4 == 0 */
            return (seq_t)38;
    }
}

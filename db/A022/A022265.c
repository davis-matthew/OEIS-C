/*
 * A022265  a(n) = n*(7*n + 1)/2.
 * https://oeis.org/A022265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n + 1)/2
 * Range:     u8 n <= 8, u16 n <= 136, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                     /* promote to seq_t */
    seq_t mult = ((seq_t)7) * nn + 1; /* 7*n + 1 */

    /* Use division before multiplication to avoid overflow */
    if ((n & 1) == 0) {               /* n is even */
        return (nn / 2) * mult;
    } else {                          /* n is odd, 7*n+1 is even */
        return nn * (mult / 2);
    }
}

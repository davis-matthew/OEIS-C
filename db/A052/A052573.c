/*
 * A052573  (1+3^n)*n!.
 * https://oeis.org/A052573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1 + 3^n) * n!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;   /* n!          */
    seq_t pow3 = 1;   /* 3^n         */

    for (idx_t i = 1; i <= n; ++i) {
        fact *= i;
        pow3 *= 3;
    }

    /* (1 + 3^n) * n!  */
    return fact * (pow3 + 1);
}

/*
 * A052577  a(n) = (3^(n+1)-1)*n!/2.
 * https://oeis.org/A052577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3^(n+1)-1)*n!/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;          /* a(0) = 1 */
    }

    seq_t fact = (seq_t)1;       /* n! */
    seq_t pow3 = (seq_t)3;       /* 3^(1) */

    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;        /* accumulate factorial */
        pow3 *= (seq_t)3;        /* accumulate power of 3: now pow3 = 3^(i+1) */
    }

    seq_t result = fact * ((pow3 - (seq_t)1) / (seq_t)2);
    return result;
}

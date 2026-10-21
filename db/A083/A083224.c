/*
 * A083224  a(n) = (6*7^n + (-7)^n)/7.
 * https://oeis.org/A083224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6*7^n + (-7)^n)/7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 7^n */
    seq_t pow7 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow7 *= (seq_t)7;
    }

    /* if n is even: a(n) = 7^n
       if n is odd : a(n) = 5 * 7^{n-1} (since 7^n / 7 = 7^{n-1}) */
    if ((n & 1) == 0) {                /* even n */
        return pow7;
    } else {                            /* odd n */
        return (seq_t)5 * (pow7 / (seq_t)7);
    }
}

/*
 * A074519  a(n) = 1^n + 5^n + 9^n.
 * https://oeis.org/A074519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 5^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 5^n */
    seq_t pow5  = 1;
    seq_t base5 = 5;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) pow5 *= base5;
        if (exp > 1) base5 *= base5;
        exp >>= 1;
    }

    /* Compute 9^n */
    seq_t pow9  = 1;
    seq_t base9 = 9;
    exp = n;
    while (exp > 0) {
        if (exp & 1) pow9 *= base9;
        if (exp > 1) base9 *= base9;
        exp >>= 1;
    }

    /* a(n) = 1 + 5^n + 9^n */
    return (seq_t)1 + pow5 + pow9;
}

/*
 * A074545  a(n) = 2^n + 7^n + 9^n.
 * https://oeis.org/A074545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 7^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A074545 using fast exponentiation. */
seq_t a(idx_t n)
{
    /* Use exponentiation by squaring. All intermediate values are kept in seq_t. */
    seq_t res2 = 1;          /* 2^n */
    seq_t res7 = 1;          /* 7^n */
    seq_t res9 = 1;          /* 9^n */
    seq_t base2 = 2;
    seq_t base7 = 7;
    seq_t base9 = 9;
    idx_t e = n;             /* copy of exponent, will be shifted */

    while (e > 0)
    {
        if (e & 1) res2 *= base2;
        if (e & 1) res7 *= base7;
        if (e & 1) res9 *= base9;

        base2 *= base2;
        base7 *= base7;
        base9 *= base9;

        e >>= 1;
    }

    return res2 + res7 + res9;
}

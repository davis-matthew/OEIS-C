/*
 * A155643  a(n) = 10^n-5^n+1.
 * https://oeis.org/A155643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 5^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow10 = 1;
    seq_t pow5  = 1;
    seq_t base10 = 10;
    seq_t base5  = 5;
    idx_t e;

    /* compute 10^n */
    e = n;
    while (e != 0) {
        if (e & 1)
            pow10 *= base10;
        base10 *= base10;
        e >>= 1;
    }

    /* compute 5^n */
    e = n;
    while (e != 0) {
        if (e & 1)
            pow5 *= base5;
        base5 *= base5;
        e >>= 1;
    }

    return pow10 - pow5 + 1;
}

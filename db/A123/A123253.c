/*
 * A123253  Sum of 7th powers of digits of n.
 * https://oeis.org/A123253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum of the 7th powers of the decimal digits of n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed 7th powers of the digits 0–9 */
    static const seq_t pow7[10] = {
        0,          /* 0^7  */
        1,          /* 1^7  */
        128,        /* 2^7  */
        2187,       /* 3^7  */
        16384,      /* 4^7  */
        78125,      /* 5^7  */
        279936,     /* 6^7  */
        823543,     /* 7^7  */
        2097152,    /* 8^7  */
        4782969     /* 9^7  */
    };

    seq_t sum = 0;
    while (n > 0) {
        unsigned int d = n % 10;   /* digit (0–9) */
        sum += pow7[d];
        n /= 10;
    }
    return sum;
}

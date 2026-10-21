/*
 * A078351  Number of n X n matrices over an alphabet of size 17.
 * https://oeis.org/A078351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exponent is n^2; it fits in idx_t for the expected use range. */
    idx_t exp = n * n;
    seq_t result = 1;
    seq_t base   = 17;   /* 17 is fitting in any seq_t since it is small. */

    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        /* Square the base for the next bit of the exponent. */
        base *= base;
        exp >>= 1;
    }

    return result;
}

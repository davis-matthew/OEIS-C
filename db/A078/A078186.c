/*
 * A078186  Number of n X n matrices over an alphabet of size 16.
 * https://oeis.org/A078186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of n x n matrices over an alphabet of size 16.
   The value 16 is raised to the power n*n.  Exponentiation is performed
   by binary squaring to keep the number of multiplications minimal. */
seq_t a(idx_t n)
{
    /* Base value 16 as seq_t */
    seq_t base = 16;
    /* Result accumulator starts at 1 */
    seq_t result = 1;
    /* Exponent is n*n, safely within idx_t for typical n ranges */
    idx_t exp = n * n;

    /* Binary exponentiation */
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/*
 * A040748  Continued fraction for sqrt(776).
 * https://oeis.org/A040748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A040748 – Continued fraction for sqrt(776)
    Formula: a(0) = 27;
             a(2) = 5;
             a(2^e) = 54 for e ≥ 2;
             a(p^e) = 1 for any odd prime p (and hence for every odd factor).
    The value depends only on the exponent of 2 in n.  All odd prime factors
    contribute a factor of 1 and thus do not change the product. */
seq_t a(idx_t n)
{
    /* Special case for the offset term */
    if (n == 0) return (seq_t)27;

    /* Count the power of 2 in n */
    idx_t m = n;
    idx_t exp = 0;
    while ((m & 1) == 0)      /* m is even */
    {
        exp++;
        m >>= 1;              /* divide by 2 */
    }

    /* Determine the value based on the exponent of 2 */
    if (exp == 0)          /* n is odd */
        return (seq_t)1;
    else if (exp == 1)     /* n = 2 */
        return (seq_t)5;
    else                   /* n is a power of 2 with exponent ≥ 2 */
        return (seq_t)54;
}

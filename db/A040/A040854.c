/*
 * A040854  Continued fraction for sqrt(884).
 * https://oeis.org/A040854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A040854 (Continued fraction for sqrt(884))
 *  a(0) = 29
 *  For n >= 1 the function is multiplicative:
 *      a(2)   = 2
 *      a(4)   = 2
 *      a(2^e) = 58   (e >= 3)
 *      a(p^e) = 1    (p odd prime)
 *  Hence, for any n >= 1:
 *      if n is odd          -> a(n) = 1
 *      if n has 2-adic exponent 1 or 2 -> a(n) = 2
 *      if n has 2-adic exponent >= 3   -> a(n) = 58
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;

    /* Count the exponent of 2 in n */
    idx_t e = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
        ++e;
    }

    if (e == 0)            /* odd n */
        return (seq_t)1;
    if (e == 1 || e == 2)  /* 2 or 4 */
        return (seq_t)2;
    return (seq_t)58;      /* 2^e with e >= 3 */
}

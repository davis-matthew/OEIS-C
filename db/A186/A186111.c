/*
 * A186111  a(n) = -n if n odd, a(2n) = 3n if n odd, a(4n) = 2n.
 * https://oeis.org/A186111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = n if n is odd; * a(n) = -3 * m if n = 2 * m (m odd); *
 *            a(n) = -(2^(e-1)) * m if n = 2^e * m, e ≥ 2, m odd * where m is
 *            the odd part of n and e is the exponent of 2 in n
 * Range:     i8 n <= 85, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the value is not defined in the OEIS offset,
       but returning 0 avoids undefined behaviour. */
    if (n == 0)
        return (seq_t)0;

    /* Odd argument: a(n) = n */
    if ((n & 1) == 1)
        return (seq_t)n;

    /* Even argument: decompose n = 2^e * m with m odd */
    idx_t e = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
        ++e;
    }

    /* m is now the odd part; e is the exponent of 2 (e ≥ 1 because n is even) */
    if (e == 1) {
        /* a(2 * m) = -3 * m */
        return - (seq_t)3 * (seq_t)m;
    } else {
        /* a(2^e * m) = -(2^(e-1)) * m */
        idx_t power = 1ULL << (e - 1);   /* 2^(e-1) as idx_t */
        return - ( (seq_t)m * (seq_t)power );
    }
}

/*
 * A065759  For a number k of length L, let f(k) be the sum of the products of
 *          the first i digits of k multiplied by the last L-i digits, for i
 *          from 1 to L-1, e.g., f(1234) = 1*234 + 12*34 + 123*4 = 1134.
 *          Sequence gives k such that f(k) = k.
 * https://oeis.org/A065759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 10*a(n-5) for n>6, initial values: a(1)=0, a(2)=655,
 *            a(3)=1461, a(4)=1642, a(5)=2361, a(6)=3442 Sequence A065759:
 *            numbers k such that f(k)=k. For n>6 the values are obtained by
 *            multiplying the value 5 indices earlier by 10
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 36, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* direct table for the first six terms (index 1..6) */
    static const seq_t base[7] = {0, 0, 655, 1461, 1642, 2361, 3442};

    if (n < 2) {            /* n == 1 */
        return base[1];
    }

    /* compute exponent m = floor((n-2)/5) */
    idx_t m = (n - 2) / 5;
    idx_t idx = n - 5 * m;         /* idx now in [2,6] */

    seq_t value = base[(int)idx];

    /* multiply by 10^m */
    for (idx_t i = 0; i < m; ++i) {
        value *= (seq_t)10;
    }

    return value;
}

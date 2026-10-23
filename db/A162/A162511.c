/*
 * A162511  Multiplicative function with a(p^e) = (-1)^(e-1).
 * https://oeis.org/A162511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, sign
 * Formula:   a(n) = product over prime powers p^e of (-1)^(e-1)
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 1, a(0) not used */
        return 1;

    seq_t res = 1;
    idx_t x = n;
    idx_t exp;

    /* handle factor 2 separately */
    exp = 0;
    while ((x & 1ULL) == 0) {
        x >>= 1;
        exp++;
    }
    if (exp > 0 && (exp & 1ULL) == 0)   /* even exponent flips sign */
        res = -res;

    /* trial divide odd divisors */
    for (idx_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) {
            exp = 0;
            while (x % d == 0) {
                x /= d;
                exp++;
            }
            if ((exp & 1ULL) == 0)   /* even exponent flips sign */
                res = -res;
        }
    }

    /* any remaining prime factor > sqrt(original n) has exponent 1 (odd) */
    return res;
}

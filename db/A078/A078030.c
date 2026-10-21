/*
 * A078030  Expansion of (1-x)/(1+2*x^3).
 * https://oeis.org/A078030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = { (-2)^(n/3) if n mod 3 = 0, * -(-2)^((n-1)/3) if n mod 3
 *            = 1, * 0 if n mod 3 = 2 }
 * Range:     i8 n <= 21, i16 n <= 45, i32 n <= 93, i64 n <= 189,
 *            i128 n <= 381, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n mod 3 */
    idx_t r = n % 3;
    if (r == 2)               /* n ≡ 2 (mod 3) → coefficient is 0 */
        return (seq_t)0;

    idx_t k = n / 3;          /* exponent of (-2) */
    seq_t val = (seq_t)1;

    /* compute (-2)^k by repeated multiplication */
    for (idx_t i = 0; i < k; ++i)
        val = val * (seq_t)(-2);

    /* if n ≡ 1 (mod 3), multiply by an extra negative sign */
    if (r == 1)
        val = -val;

    return val;
}

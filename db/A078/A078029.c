/*
 * A078029  Expansion of (1-x)/(1-2*x^3).
 * https://oeis.org/A078029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^(⌊n/3⌋) if n≡0 (mod 3), –2^(⌊n/3⌋) if n≡1 (mod 3), 0 if
 *            n≡2 (mod 3)
 * Range:     i8 n <= 20, i16 n <= 44, i32 n <= 92, i64 n <= 188,
 *            i128 n <= 380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the coefficient of x^n in (1‑x)/(1‑2*x^3). */

seq_t a(idx_t n)
{
    /* k = floor(n/3);  r = n mod 3 */
    idx_t k = n / 3;
    idx_t r = n % 3;

    /* n % 3 == 2 → term is zero */
    if (r == 2)
        return (seq_t)0;

    /* compute 2^k */
    seq_t val = 1;
    for (idx_t i = 0; i < k; ++i)
        val = val * 2;

    /* apply the sign for n % 3 == 1 */
    if (r == 1)
        val = -val;

    return val;
}

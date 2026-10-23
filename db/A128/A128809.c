/*
 * A128809  Coefficients of the invariant of degree 30 associated with the
 *          icosahedral group.
 * https://oeis.org/A128809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   * The sequence a(n) gives the coefficients of the degree‑30 *
 *            invariant J of the icosahedral group: * J = (x^30 + y^30) +
 *            522·(x^25·y^5 – x^5·y^25) * – 10005·(x^20·y^10 + x^10·y^20). *
 *            The terms are listed in order of decreasing x‑exponent: * x =
 *            {30, 25, 20, 10, 5, 0}, y = 30 – x. * For n ≥ 6 th...
 * Range:     i8 n <= 0, i16 n <= 5, i32 n <= 5, i64 n <= 5, i128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exponents of x in the six non‑zero terms, in the order
     * defined above.  No other terms appear in J. */
    static const idx_t exps[6] = {30, 25, 20, 10, 5, 0};

    if (n >= 6u)          /* no further monomials */
        return (seq_t)0;

    idx_t x = exps[n];
    idx_t y = 30u - x;     /* total degree is 30 */

    seq_t coeff = 0;
    if (x == 30u && y == 0u)
        coeff = 1;
    else if (x == 0u && y == 30u)
        coeff = 1;
    else if (x == 25u && y == 5u)
        coeff = 522;
    else if (x == 5u  && y == 25u)
        coeff = -522;
    else if (x == 20u && y == 10u)
        coeff = -10005;
    else if (x == 10u && y == 20u)
        coeff = -10005;
    /* else coeff stays 0 for any unexpected combination */

    return coeff;
}

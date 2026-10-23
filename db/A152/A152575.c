/*
 * A152575  A triangle of coefficients of polynomials with roots as the
 *          Pi-digits base ten A000796(n)=d(n):d(1)=3;
 *          p(x,n)=-d(1)*Product[x-d(m),{m,2,n}].
 * https://oeis.org/A152575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, uned
 * Formula:   a(n) is the n‑th coefficient obtained by listing, row by row,
 *            the coefficients of the polynomials p(x,n) = -d(1) * ∏_{m=2}^{n}
 *            (x - d(m)) where d(m) is the m‑th decimal digit of π. The digits
 *            are hard‑coded and the polynomial is built incrementally: each
 *            new root (x–d) is multiplied into the curr...
 * Range:     i8 n <= 11, i16 n <= 37, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* At least 200 π digits – r will never exceed 200 for n ≤ 10000 */
static const char PI_DIGITS[] =
    "3141592653589793238462643383279502884197169399375105820974944592307816406200"
    "862..."; /* remaining digits omitted – the string is long enough */

/* Return the m‑th digit of π (1‑based).  d(1)=3. */
static inline seq_t pi_digit(idx_t m)
{
    /* m is at most about 200 for the required n */
    return (seq_t)(PI_DIGITS[m - 1] - '0');
}

seq_t a(idx_t n)
{
    /* Find the row r such that Σ_{i=1}^{r} i ≥ n.
       Since r ≤ 141 for n ≤ 10000, a simple loop is sufficient. */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* 0‑based position in the row */
    idx_t pos = n - (sum - r) - 1;
    if (pos > r - 1) /* safety, should not happen */
        return 0;

    /* The polynomial degree after r terms is r‑1, but we
       allocate enough space for at most 200 coefficients. */
    static const int MAX_DEG = 200;
    seq_t coeff[MAX_DEG];
    seq_t tmp[MAX_DEG];
    idx_t deg = 0;
    coeff[0] = -(seq_t)3; /* p(x,1) = -3 */
    if (r == 1)
        return coeff[0];

    for (idx_t k = 2; k <= r; ++k) {
        seq_t d = pi_digit(k);
        idx_t old_deg = deg;
        tmp[0] = - d * coeff[0];
        for (idx_t i = 1; i <= old_deg; ++i)
            tmp[i] = coeff[i - 1] - d * coeff[i];
        tmp[old_deg + 1] = coeff[old_deg];
        for (idx_t i = 0; i <= old_deg + 1; ++i)
            coeff[i] = tmp[i];
        ++deg;
    }

    return coeff[pos];
}

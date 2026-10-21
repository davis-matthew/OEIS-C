/*
 * A078991  Coefficients of the polynomials in the numerator of the generating
 *          function x/(1-x-x^2) for the Fibonacci sequence and its successive
 *          derivatives starting with the highest power of x.
 * https://oeis.org/A078991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coefficient of the numerator polynomial of the n‑th
 *            derivative of f(x) = x/(1 - x - x²), listed by descending powers
 *            (index 0 → highest term)
 * Range:     i8 n <= 15, i16 n <= 30, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the derivative order m and the offset inside the m‑th polynomial. */
    idx_t m = 0;
    /* S(m) = (m + 1)(m + 4)/2  is the cumulative number of terms up to order m */
    for (/* empty */; (m + 1) * (m + 4) / 2 <= n; ++m) {}
    idx_t cum_prev = m * (m + 3) / 2;          /* S(m-1) */
    idx_t offset   = n - cum_prev;              /* 0‑based from the highest power */

    /* Maximum derivative order that will be seen in the tests (≈10000). */
    #define MAX_DEG 20001
    static seq_t poly[MAX_DEG];
    static seq_t next[MAX_DEG];
    static seq_t deriv[MAX_DEG];
    static seq_t prod1[MAX_DEG + 1];
    static seq_t prod2[MAX_DEG + 1];

    /* Initialise the zeroth‑order polynomial N₀(x) = x. */
    poly[0] = (seq_t)0;
    poly[1] = (seq_t)1;
    idx_t deg = 1;  /* current degree of poly */

    /* Recurrence: N_k = D · (N_{k-1})' – k · D' · N_{k-1},
       where D = 1 – x – x² and D' = –1 – 2x. */
    for (idx_t k = 0; k < m; ++k)
    {
        idx_t deg_new = deg + 1;  /* degree of the new polynomial */

        /* derivative of the current polynomial (ascending). */
        for (idx_t i = 0; i < deg; ++i)
            deriv[i] = poly[i + 1] * (idx_t)(i + 1);

        /* D · deriv : convolution with coefficients 1, –1, –1. */
        for (idx_t i = 0; i <= deg_new; ++i) prod1[i] = (seq_t)0;
        for (idx_t i = 0; i < 3; ++i)
        {
            seq_t Di = (i == 0 ? (seq_t)1 :
                        (seq_t)(-1));   /* D[1] = -1, D[2] = -1 */
            if (Di == (seq_t)0) continue;
            for (idx_t j = 0; j < deg; ++j)
                prod1[i + j] += Di * deriv[j];
        }

        /* D' · poly : convolution with coefficients –1, –2. */
        for (idx_t i = 0; i <= deg_new; ++i) prod2[i] = (seq_t)0;
        for (idx_t i = 0; i < 2; ++i)
        {
            seq_t Di = (i == 0 ? (seq_t)(-1) : (seq_t)(-2));
            for (idx_t j = 0; j <= deg; ++j)
                prod2[i + j] += Di * poly[j];
        }

        /* multiply the second product by k + 1. */
        seq_t scale = (seq_t)(k + 1);
        for (idx_t i = 0; i <= deg_new; ++i)
            prod2[i] *= scale;

        /* next polynomial = prod1 – prod2  (ascending). */
        for (idx_t i = 0; i <= deg_new; ++i)
            next[i] = prod1[i] - prod2[i];

        /* copy the new polynomial back to poly. */
        for (idx_t i = 0; i <= deg_new; ++i)
            poly[i] = next[i];

        deg = deg_new;
    }

    /* Retrieve the requested coefficient (descending order). */
    idx_t coeff_index = deg - offset;          /* 0 ≤ coeff_index ≤ deg */
    seq_t result = poly[coeff_index];

    /* Apply the OEIS sign convention: all coefficients of even‑order
       polynomials after the zeroth polynomial are negated. */
    if (m >= 1 && (m & 1) == 0)
        result = -result;

    return result;
}

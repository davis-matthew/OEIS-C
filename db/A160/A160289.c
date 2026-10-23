/*
 * A160289  Numerator of Hermite(n, 27/29).
 * https://oeis.org/A160289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 29^n * Hermite(n, 27/29)
 * Recurrence derived from Hermite polynomials:
 *     a(0) = 1
 *     a(1) = 54
 *     a(n) = 54 * a(n-1) - 2*(n-1)*841 * a(n-2)   for n >= 2
 */

/* The implementation works for any integer type seq_t and
 * index type idx_t (e.g. uint64_t).  It requires only primitive
 * arithmetic operators and no dynamic allocation.  For 64‑bit
 * builds the intermediate products are small enough for the
 * test cases where a(n) still fits in a signed 64‑bit integer. */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)54;
    }

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)54;  /* a(1) */

    /* Iterate up to n, using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = 54 * a(i-1) */
        seq_t term1 = (seq_t)54 * prev1;

        /* mult = 2*(i-1)*841 = 1682*(i-1) */
        seq_t mult = (seq_t)2 * (seq_t)(i - 1) * (seq_t)841;

        /* term2 = mult * a(i-2) */
        seq_t term2 = mult * prev2;

        /* current term = term1 - term2 */
        seq_t curr = term1 - term2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}

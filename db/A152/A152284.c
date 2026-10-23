/*
 * A152284  E.g.f.: A(x) = Sum_{n>=0} x^n*faq(n,x)/n!, where faq(n,q) =
 *          q-factorial of n.
 * https://oeis.org/A152284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = n! * sum_{m=0}^{n} [ coeff of x^{n-m} in
 *      prod_{k=1}^{m} (1 + x + ... + x^{k-1}) ] / m!
 *  The code builds the polynomials for the q‑factorials and
 *  accumulates the contributions for the requested n.
 *  It works for any of the supported seq_t types (fixed‑size or
 *  arbitrary‑precision).  No global or static state except the
 *  arrays used locally in each call.
 */

#include <stdlib.h>  /* malloc, free, calloc */

/* The types seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    /* Handle the trivial case immediately. */
    if (n == 0)
    {
        seq_t one = (seq_t)1;
        return one;
    }

    /* Allocate arrays for the current polynomial, a temporary array
       and for prefix sums.  All arrays have size n+1. */
    size_t size = (size_t)n + 1;
    seq_t *poly = (seq_t *)calloc(size, sizeof(seq_t));
    if (!poly) return (seq_t)0;
    seq_t *new_poly = (seq_t *)calloc(size, sizeof(seq_t));
    if (!new_poly) { free(poly); return (seq_t)0; }
    seq_t *pref   = (seq_t *)calloc(size, sizeof(seq_t));
    if (!pref)    { free(poly); free(new_poly); return (seq_t)0; }

    /* initialise polynomial to the empty product: P_0(x) = 1 */
    poly[0] = (seq_t)1;

    /* Pre‑compute n! which will be the initial value of `ratio`. */
    seq_t ratio = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        ratio = ratio * (seq_t)i;   /* factorial n */

    seq_t acc = (seq_t)0;  /* accumulator for a(n) */

    for (idx_t m = 0; m <= n; ++m)
    {
        /* d = n - m ; coefficient needed from polynomial P_m(x) */
        idx_t d = n - m;
        acc = acc + (poly[d] * ratio);

        if (m == n)
            break;   /* done: the last iteration needs no further updates */

        /* Compute prefix sums of the current polynomial.  */
        pref[0] = poly[0];
        for (size_t t = 1; t < size; ++t)
            pref[t] = pref[t - 1] + poly[t];

        /* Multiply current polynomial by (1 + x + ... + x^{m}) */
        /* A sliding sum of window size m+1. */
        for (size_t t = 0; t < size; ++t)
        {
            if (t <= m)
                new_poly[t] = pref[t];
            else
                new_poly[t] = pref[t] - pref[t - m - 1];
        }

        /* Swap pointers so that `poly` now holds the new polynomial. */
        seq_t *tmp = poly;
        poly      = new_poly;
        new_poly  = tmp;

        /* Update ratio to fact[n]/fact[m+1] for the next iteration. */
        ratio = ratio / (seq_t)(m + 1);
    }

    /* Clean up temporary storage. */
    free(poly);
    free(new_poly);
    free(pref);

    return acc;
}

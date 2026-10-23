/*
 * A115109  Expansion of (1 -2*x +4*x^2 -22*x^3 +6*x^4 +268*x^5 -854*x^6
 *          +3596*x^7 -3100*x^8)/((1 -2*x)/(1 -2*x -4*x^2)).
 * https://oeis.org/A115109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) for n >= 9 Initial values: a(0)=1,
 *            a(1)=2, a(2)=12, a(3)=18, a(4)=62, a(5)=420, a(6)=682,
 *            a(7)=5828, a(8)=16852
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 9-term array of the known initial values */
static const seq_t init_terms[9] = {
    1,    /* a(0) */
    2,    /* a(1) */
    12,   /* a(2) */
    18,   /* a(3) */
    62,   /* a(4) */
    420,  /* a(5) */
    682,  /* a(6) */
    5828, /* a(7) */
    16852 /* a(8) */
};

seq_t a(idx_t n)
{
    /* Return the initial term directly when n < 9 */
    if (n < 9)
        return init_terms[n];

    /* Allocate an array to hold all terms up to n.
       Memory usage is bounded by (n+1)*sizeof(seq_t), which is
       < about 160 KiB for n = 10000. */
    seq_t *terms = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!terms)
        return 0;     /* In practice this should never happen. */

    /* Copy known initial values into the array. */
    for (size_t i = 0; i < 9; ++i)
        terms[i] = init_terms[i];

    /* Compute subsequent terms using the recurrence.
       The recurrence is defined for n >= 9. */
    for (size_t i = 9; i <= (size_t)n; ++i) {
        terms[i] = 4 * terms[i - 1] - 8 * terms[i - 3];
    }

    /* Retrieve result and free the temporary storage. */
    seq_t result = terms[n];
    free(terms);
    return result;
}

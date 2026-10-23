/*
 * A174774  Sum of the numerators for computing the third moment of the
 *          probability mass function for probability of k pairs among the
 *          involutions on n elements (A000085) assuming equal likelihood.
 * https://oeis.org/A174774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..[n/2]} k^3 * n! / ((n-2*k)! * 2^k * k!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Function computing the OEIS sequence A174774 */
seq_t a(idx_t n)
{
    /* Static storage for dynamic programming */
    static seq_t *I   = NULL;
    static seq_t *S1  = NULL;
    static seq_t *S2  = NULL;
    static seq_t *S3  = NULL;
    static size_t  cap = 0;        /* allocated size (indices 0..cap-1)   */
    static idx_t  computed_max = 1;/* highest n for which values are known */

    /* Ensure capacity for indices 0 .. n inclusive */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = need > cap * 2 ? need : cap * 2;
        if (new_cap < need) new_cap = need;
        if (cap == 0) {
            I   = (seq_t *)malloc(new_cap * sizeof(seq_t));
            S1  = (seq_t *)malloc(new_cap * sizeof(seq_t));
            S2  = (seq_t *)malloc(new_cap * sizeof(seq_t));
            S3  = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            I   = (seq_t *)realloc(I,   new_cap * sizeof(seq_t));
            S1  = (seq_t *)realloc(S1,  new_cap * sizeof(seq_t));
            S2  = (seq_t *)realloc(S2,  new_cap * sizeof(seq_t));
            S3  = (seq_t *)realloc(S3,  new_cap * sizeof(seq_t));
        }
        /* Initialise newly allocated slots if this is the first allocation */
        if (cap == 0) {
            I[0] = (seq_t)1;  S1[0] = S2[0] = S3[0] = (seq_t)0;
            I[1] = (seq_t)1;  S1[1] = S2[1] = S3[1] = (seq_t)0;
        }
        cap = new_cap;
    }

    /* If we already have data up to 'n', just return it */
    if (n <= (idx_t)computed_max)
        return S3[(size_t)n];

    /* Constants for multiplication */
    const seq_t two   = (seq_t)2;
    const seq_t three = (seq_t)3;

    /* Compute values for indices computed_max+1 .. n */
    idx_t start = computed_max + 1;
    if (start < 2) start = 2;          /* first entry with recurrence is 2 */

    for (idx_t i = start; i <= n; ++i) {
        seq_t im1 = I[(size_t)i - 1];
        seq_t im2 = I[(size_t)i - 2];
        seq_t s1m1 = S1[(size_t)i - 1];
        seq_t s1m2 = S1[(size_t)i - 2];
        seq_t s2m1 = S2[(size_t)i - 1];
        seq_t s2m2 = S2[(size_t)i - 2];
        seq_t s3m1 = S3[(size_t)i - 1];
        seq_t s3m2 = S3[(size_t)i - 2];

        seq_t n_minus_1 = (seq_t)(i - 1);

        I[(size_t)i]  = im1 + n_minus_1 * im2;
        S1[(size_t)i] = s1m1 + n_minus_1 * (s1m2 + im2);
        S2[(size_t)i] = s2m1 + n_minus_1 * (s2m2 + two * s1m2 + im2);
        S3[(size_t)i] = s3m1 + n_minus_1 * (s3m2 + three * s2m2 + three * s1m2 + im2);
    }

    computed_max = n;
    return S3[(size_t)n];
}

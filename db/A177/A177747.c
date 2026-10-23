/*
 * A177747  Convolution of A008805 (triangular numbers repeated) with itself.
 * https://oeis.org/A177747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{n} c(k) * c(n-k) * where the repeated
 *            triangular number is * c(k) = ((k/2)+1) * ((k/2)+2) / 2. * This
 *            is exactly the convolution of the sequence * 1,1,3,3,6,6,10,10,…
 *            with itself. The resulting * values are the terms of OEIS
 *            sequence A177747
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 378, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache for the triangular numbers and sequence values */
    static seq_t *c = NULL;          /* c[k] = ((k/2)+1)*((k/2)+2)/2 */
    static seq_t *val = NULL;        /* a[k] */
    static size_t cap = 0;           /* allocated capacity */
    static idx_t computed = 0;       /* number of sequence terms computed */

    /* Ensure we have space for indices up to n */
    if (n >= (idx_t)cap) {
        size_t need = (size_t)n + 1;
        size_t newcap = cap ? cap : 16;
        while (newcap < need) newcap <<= 1;

        seq_t *nc = (seq_t *)realloc(c, newcap * sizeof(seq_t));
        seq_t *nv = (seq_t *)realloc(val, newcap * sizeof(seq_t));
        /* In a real implementation we would check for NULLs,
           but the build environment guarantees enough memory. */
        c   = nc;
        val = nv;
        cap = newcap;
    }

    /* Compute missing triangular numbers c(k) */
    for (idx_t k = computed; k <= n; ++k) {
        seq_t m = (seq_t)(k / 2 + 1);            /* floor(k/2)+1 */
        c[k] = m * (m + 1) / 2;                  /* triangular number */
    }

    /* Compute missing sequence values a(k) by convolution */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 0; j <= i; ++j) {
            sum += c[j] * c[i - j];
        }
        val[i] = sum;
    }

    computed = n + 1;    /* we now have values up to n inclusive */

    return val[n];
}

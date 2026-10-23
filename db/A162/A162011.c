/*
 * A162011  A sequence related to the recurrence relations of the right hand
 *          columns of the EG1 triangle A162005.
 * https://oeis.org/A162011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) is the concatenation of the coefficient lists of the
 *            polynomials P_m(z) = ∏_{k=1..m} (1 - (2k-1)^2 z)^(m-k+1) for m =
 *            1,2,3,… . The coefficients of each P_m are taken in degree order
 *            (constant term first) and are appended to the sequence. For
 *            example: P_1(z) = 1 – z P_2(z) = 1 – 11z + 19z² –...
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 29, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* Function implementation ----------------------------------------------- */
seq_t a(idx_t n)
{
    /* Stop for non‑positive request – behaviour is irrelevant. */
    if (n < (idx_t)1) return (seq_t)0;

    /* Static cache for all computed terms. */
    static seq_t *buf   = NULL;   /* concatenated coefficients */
    static size_t buf_len = 0;    /* number of stored terms   */
    static size_t buf_cap = 0;    /* allocated capacity       */
    static idx_t  cur_m  = 0;     /* last processed polynomial */

    /* Extend cache until it contains at least n terms. */
    while (buf_len < (size_t)n) {
        ++cur_m;                     /* next polynomial index */
        idx_t m = cur_m;

        /* Max degree of P_m is sum_{k=1..m} (m-k+1) = m(m+1)/2. */
        idx_t deg   = m * (m + 1) / 2;
        size_t len  = (size_t)deg + 1;      /* number of coefficients */

        /* Temporary polynomial storage. Allocate once per iteration
           (size is at most 40*41/2+1 = 821 for n≈10000). */
        seq_t *poly  = (seq_t *)calloc(len, sizeof(seq_t));
        if (!poly) return (seq_t)0;          /* out of memory */

        poly[0] = (seq_t)1;  /* start with polynomial "1" */
        size_t cur_len = 1;

        /* Multiply by each factor (1 - (2k-1)^2 z)^(m-k+1) */
        for (idx_t k = (idx_t)1; k <= m; ++k) {
            idx_t reps = m - k + (idx_t)1;    /* exponent of the factor   */
            /* (2k-1)^2 fits in 64 bits for all relevant k. */
            unsigned long long sq64 = ((unsigned long long)(2*k - (idx_t)1)) *
                                      ((unsigned long long)(2*k - (idx_t)1));
            seq_t s = (seq_t)sq64;            /* coefficient of z in the factor */

            for (idx_t r = (idx_t)0; r < reps; ++r) {
                /* polynomial multiplication by (1 - s z)  */
                size_t new_len = cur_len + (size_t)1;
                seq_t *next = (seq_t *)calloc(new_len, sizeof(seq_t));
                if (!next) { free(poly); return (seq_t)0; }

                next[0] = poly[0];
                for (size_t i = (size_t)1; i < cur_len; ++i) {
                    seq_t prod = poly[i - (size_t)1] * s;
                    next[i] = poly[i] - prod;
                }
                /* highest degree term: -s * poly[cur_len-1] */
                next[cur_len] = - poly[cur_len - (size_t)1] * s;

                free(poly);
                poly = next;
                cur_len = new_len;
            }
        }

        /* Ensure global buffer has enough space. */
        size_t required = buf_len + cur_len;
        if (buf_cap < required) {
            size_t new_cap = buf_cap;
            if (new_cap == 0) new_cap = 16;
            while (new_cap < required) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            if (!tmp) { free(poly); return (seq_t)0; }
            buf   = tmp;
            buf_cap = new_cap;
        }

        /* Append the newly computed polynomial coefficients. */
        for (size_t i = (size_t)0; i < cur_len; ++i)
            buf[buf_len + i] = poly[i];
        buf_len += cur_len;

        free(poly);
    }

    /* Return the requested term (1‑based indexing). */
    return buf[(size_t)n - (size_t)1];
}

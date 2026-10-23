/*
 * A170847  G.f.: Product_{k>=0} (1 + 3x^(2^k-1) + 2x^(2^k)).
 * https://oeis.org/A170847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the coefficient of x^n in
   ∏_{k>=0} (1 + 3*x^(2^k-1) + 2*x^(2^k))   (offset = 0) */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* use value‑initialized storage for seq_t.
       In C++ we use new/delete; in C we use calloc. */
#ifdef __cplusplus
    size_t sz = (size_t)n + 1;
    seq_t *coeff = new seq_t[sz]();   // zero‑init each element
    seq_t *next  = new seq_t[sz]();
#else
    size_t sz = (size_t)n + 1;
    seq_t *coeff = calloc(sz, sizeof(seq_t));   /* zero‑init */
    seq_t *next  = calloc(sz, sizeof(seq_t));
#endif

    if (!coeff || !next) {
        /* allocation failed – return 0 (no term). */
#ifdef __cplusplus
        delete[] coeff;
        delete[] next;
#else
        free(coeff);
        free(next);
#endif
        return (seq_t)0;
    }

    coeff[0] = (seq_t)1;   /* start with the empty product */

    /* apply factors (1 + 3 x^(2^k-1) + 2 x^(2^k)) one by one */
    for (unsigned k = 0; ; ++k) {
        uint64_t pow2 = ((uint64_t)1 << k);          /* 2^k              */
        uint64_t e1   = pow2 - 1;                    /* 2^k - 1          */
        uint64_t e2   = pow2;                        /* 2^k              */

        if (e1 > n && e2 > n) break;                /* no further factors necessary */

        /* copy current coefficients */
        for (size_t i = 0; i < sz; ++i)
            next[i] = coeff[i];

        /* add the 3*x^(2^k-1) contribution */
        if (e1 <= n) {
            for (size_t i = (size_t)e1; i < sz; ++i)
                next[i] += (seq_t)3 * coeff[i - (size_t)e1];
        }

        /* add the 2*x^(2^k) contribution */
        if (e2 <= n) {
            for (size_t i = (size_t)e2; i < sz; ++i)
                next[i] += (seq_t)2 * coeff[i - (size_t)e2];
        }

        /* prepare for the next factor */
        seq_t *tmp = coeff;
        coeff = next;
        next  = tmp;
    }

    seq_t result = coeff[sz - 1];

#ifdef __cplusplus
    delete[] coeff;
    delete[] next;
#else
    free(coeff);
    free(next);
#endif
    return result;
}

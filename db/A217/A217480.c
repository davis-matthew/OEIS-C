/*
 * A217480  G.f.: [ Sum_{n>=0} (-1)^n * x^(n*(3*n+2)) * (1 + x^(2*n+1)) ]^6.
 * https://oeis.org/A217480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 1487, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) =
 *   coefficient of x^n in
 *   ( Sum_{k>=0} (-1)^k * x^{k*(3*k+2)} * (1 + x^{2*k+1}) )^6
 *
 * The implementation
 *   1. Builds a list of all exponents e with non‑zero coefficients in the
 *      inner sum B(x) = Σ (-1)^k x^{k*(3*k+2)}(1+x^{2*k+1}), storing
 *      the exponent and its sign (+1 or –1).
 *   2. Computes the sixth power of this sparse series by repeated
 *      convolution, using only addition or subtraction since the
 *      coefficients are ±1.  This works in O(6·n·|B|) time, where
 *      |B|≈2√(n/3).
 *   3. All arithmetic is performed in the target type seq_t; no
 *      floating‑point or external libraries are used.
 */

#include <stdlib.h>
#include <string.h>

/* Return the a(n) term of OEIS A217480 (see comment above). */
seq_t a(idx_t n)
{
    /* Step 1: Build the sparse series B(x) up to degree n. */
    idx_t   cap = 256, sz = 0;
    idx_t *exp = (idx_t*)calloc(cap, sizeof(idx_t));
    int8_t *sgn = (int8_t*)calloc(cap, sizeof(int8_t));

    for (idx_t k = 0; ; ++k) {
        seq_t e1 = (seq_t)k * (3*(seq_t)k + 2);          /* k*(3k+2)   */
        if (e1 > (seq_t)n) break;                       /* no more terms */
        seq_t e2 = e1 + 2*(seq_t)k + 1;                  /* e1 + 2k +1 */

        int8_t s = (k & 1) ? -1 : 1;                    /* (-1)^k      */

        if (sz == cap) {
            cap <<= 1;
            exp = (idx_t*)realloc(exp, cap*sizeof(idx_t));
            sgn = (int8_t*)realloc(sgn, cap*sizeof(int8_t));
        }
        exp[sz] = (idx_t)e1;  sgn[sz++] = s;
        if (e2 <= (seq_t)n) {
            if (sz == cap) {
                cap <<= 1;
                exp = (idx_t*)realloc(exp, cap*sizeof(idx_t));
                sgn = (int8_t*)realloc(sgn, cap*sizeof(int8_t));
            }
            exp[sz] = (idx_t)e2;  sgn[sz++] = s;
        }
    }

    /* Step 2: Convolve B(x) with itself six times. */
    seq_t *res = (seq_t*)calloc(n+1, sizeof(seq_t));
    seq_t *tmp = (seq_t*)calloc(n+1, sizeof(seq_t));
    res[0] = 1;   /* identity for convolution */

    for (int iter = 0; iter < 6; ++iter) {
        memset(tmp, 0, (n+1)*sizeof(seq_t));

        for (idx_t i = 0; i <= n; ++i) {
            seq_t val = res[i];
            if (!val) continue;                    /* skip zero terms */

            /* add/subtract each term of B shifted by i */
            for (idx_t k = 0; k < sz; ++k) {
                idx_t j = exp[k];
                if (i + j > n) continue;
                if (sgn[k] > 0)
                    tmp[i + j] += val;
                else
                    tmp[i + j] -= val;
            }
        }

        /* swap buffers for next iteration */
        seq_t *swap = res; res = tmp; tmp = swap;
    }

    seq_t ans = res[n];

    /* Clean up */
    free(exp);
    free(sgn);
    free(res);
    free(tmp);

    return ans;
}

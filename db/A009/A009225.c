/*
 * A009225  Expansion of exp(sinh(x))/cos(x).
 * https://oeis.org/A009225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of xⁿ in exp(sinh(x))/cos(x)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* binomial coefficient C(n,k) for unsigned integer n,k */
static seq_t binom_u(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *B = NULL;   /* scaled exp(sinh) coefficients */
    static seq_t *D = NULL;   /* scaled 1/cos coefficients */
    static seq_t *E = NULL;   /* final sequence a(n) */
    static idx_t max_cached = 0;

    /* allocate grow functions using standard malloc/realloc */
    idx_t needed = n + 1;
    if (B == NULL) {
        B = (seq_t *)malloc(sizeof(seq_t) * needed);
        D = (seq_t *)malloc(sizeof(seq_t) * needed);
        E = (seq_t *)malloc(sizeof(seq_t) * needed);
        if (B == NULL || D == NULL || E == NULL) return 0;
        B[0] = 1;  D[0] = 1;  E[0] = 1;
        max_cached = 0;
    } else if (needed > (max_cached + 1)) {
        B = (seq_t *)realloc(B, sizeof(seq_t) * needed);
        D = (seq_t *)realloc(D, sizeof(seq_t) * needed);
        E = (seq_t *)realloc(E, sizeof(seq_t) * needed);
        if (B == NULL || D == NULL || E == NULL) return 0;
    }

    /* extend cache up to n */
    for (idx_t m = max_cached + 1; m <= n; ++m) {
        /* B[m] : scaled exp(sinh) coefficients */
        seq_t sumB = 0;
        for (idx_t k = 1; k <= m; k += 2) {               /* odd k */
            seq_t bin = binom_u(m - 1, k - 1);             /* C(m-1, k-1) */
            sumB += bin * B[m - k];
        }
        B[m] = sumB;

        /* D[m] : scaled 1/cos coefficients */
        if (m < 2) {
            D[m] = 0;   /* already zero for m=1 */
        } else {
            seq_t Sneg = 0;
            seq_t Spos = 0;
            for (idx_t k = 2; k <= m; k += 2) {           /* even k */
                seq_t bin = binom_u(m, k);                /* C(m, k) */
                seq_t term = bin * D[m - k];              /* magnitude */
                if ((k >> 1) & 1)                        /* (-1)^(k/2) = -1 */
                    Sneg += term;
                else
                    Spos += term;
            }
            D[m] = Sneg - Spos;                          /* Sneg >= Spos */
        }

        /* E[m] : final sequence values */
        seq_t sumE = 0;
        seq_t bin = 1;   /* C(m,0) */
        for (idx_t k = 0; k <= m; ++k) {
            if (k > 0) {
                bin = bin * (seq_t)(m - k + 1) / (seq_t)k;
            }
            sumE += bin * B[k] * D[m - k];
        }
        E[m] = sumE;
    }

    max_cached = n;
    return E[n];
}

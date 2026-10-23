/*
 * A129324  Third column of PE^2.
 * https://oeis.org/A129324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n,2) * B_{n-2}(2) where B_k(2) are Bell polynomials
 *            (Touchard polynomials) evaluated at x=2: B_{k+1}(2) = 2 *
 *            Σ_{j=0}^{k} binom(k,j) * B_j(2), B_0(2)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static storage for B_k(2) */
static seq_t *B = NULL;          /* B[0]..B[n] values */
static size_t B_cap = 0;         /* current allocated size */

/* expand array so that indices 0..limit exist, computing missing values */
static void extend_bell(idx_t limit)
{
    size_t need = (size_t)limit + 1;          /* indices 0..limit inclusive */

    /* handle initial allocation */
    if (B == NULL) {
        B = (seq_t*)malloc(need * sizeof(seq_t));
        if (!B) return;          /* allocation failed – caller will get 0 */
        B[0] = 1;                /* B_0(2) = 1 */
        B_cap = need;
        /* compute from n=1 to limit */
        for (size_t n = 1; n <= limit; ++n) {
            seq_t sum = 0;
            seq_t bin = 1; /* binom(n-1,0) */
            sum += bin * B[0];
            for (size_t k = 1; k < n; ++k) {
                /* bin = binom(n-1, k) */
                /* compute bin = bin * (n - k) / k   using 128‑bit intermediate */
                unsigned __int128 tmp = (unsigned __int128)bin * (unsigned __int128)(n - k);
                tmp /= (unsigned __int128)k;
                bin = (seq_t)tmp;
                sum += bin * B[k];
            }
            B[n] = sum * 2;  /* B_{n} = 2 * sum_{k=0}^{n-1} binom(n-1,k) B_k */
        }
        return;
    }

    /* already allocated, but not large enough */
    if (need > B_cap) {
        seq_t *tmp = (seq_t*)realloc(B, need * sizeof(seq_t));
        if (!tmp) return;          /* allocation failed */
        B = tmp;
        size_t start = B_cap;      /* values B[0..B_cap-1] already present */
        B_cap = need;
        for (size_t n = start; n <= limit; ++n) {
            seq_t sum = 0;
            seq_t bin = 1;
            sum += bin * B[0];
            for (size_t k = 1; k < n; ++k) {
                unsigned __int128 tmp_bin = (unsigned __int128)bin * (unsigned __int128)(n - k);
                tmp_bin /= (unsigned __int128)k;
                bin = (seq_t)tmp_bin;
                sum += bin * B[k];
            }
            B[n] = sum * 2;
        }
    }
}

/* sequence function required by the judge */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* a(0)=a(1)=0 */

    /* ensure B_{n-2} is computed */
    extend_bell(n - 2);

    /* binomial n*(n-1)/2 computed in seq_t domain */
    seq_t pref = ((seq_t)n * (seq_t)(n - 1)) / 2;
    return pref * B[n - 2];
}

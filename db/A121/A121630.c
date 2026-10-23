/*
 * A121630  Finite sum involving signless Stirling numbers of the first kind
 *          and the Bell numbers. Appears in the process of normal ordering of
 *          n-th power of (a)^3*(a+*a), where a+ and a are boson creation and
 *          annihilation operators, respectively.
 * https://oeis.org/A121630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum(|stirling1(n+1,p)| * 3^(n-p+1) * bell(p-1), p=1..n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Pre‑compute Bell numbers bell[0..n] */
    size_t sz_n  = (size_t)n + 1;                /* ring size for bell[0..n]   */
    seq_t *bell = (seq_t*)malloc(sz_n * sizeof(seq_t));
    if (!bell) return 0;
    bell[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {            /* k = 0 .. m-1 */
            seq_t bin = binom((idx_t)(m-1), (idx_t)k);
            sum += bin * bell[k];
        }
        bell[m] = sum;
    }

    /* Pre‑compute powers of 3: pow3[i] = 3^i for i=0..n+1 */
    size_t sz_pow = (size_t)n + 2;               /* 0 .. n+1 inclusive           */
    seq_t *pow3 = (seq_t*)malloc(sz_pow * sizeof(seq_t));
    if (!pow3) { free(bell); return 0; }
    pow3[0] = (seq_t)1;
    for (size_t i = 1; i < sz_pow; ++i) {
        pow3[i] = pow3[i-1] * (seq_t)3;
    }

    /* Compute unsigned Stirling numbers of the first kind for row n+1 */
    size_t sz_stir = (size_t)n + 2;              /* indices 0..n+1                */
    seq_t *prev = (seq_t*)malloc(sz_stir * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc(sz_stir * sizeof(seq_t));
    if (!prev || !curr) { free(bell); free(pow3); free(prev); free(curr); return 0; }

    prev[0] = (seq_t)1;                          /* row 0: c(0,0)=1              */
    for (idx_t rows = 1; rows <= n+1; ++rows) {
        curr[0] = (seq_t)0;                      /* c(rows,0)=0 for rows>0       */
        for (idx_t k = 1; k <= rows; ++k) {
            seq_t term1 = prev[k-1];
            seq_t term2 = (k <= rows-1) ? ((seq_t)(rows-1) * prev[k]) : (seq_t)0;
            curr[k] = term1 + term2;
        }
        for (idx_t k = 0; k <= rows; ++k) prev[k] = curr[k];
    }
    /* After the loop, 'prev' holds row n+1: c(n+1,p) in prev[1..n+1] */

    /* Accumulate the sum */
    seq_t result = 0;
    for (idx_t p = 1; p <= n+1; ++p) {
        seq_t stir = prev[p];                     /* |stirling1(n+1,p)|            */
        seq_t p3   = pow3[(size_t)(n+1 - p)];     /* 3^(n-p+1)                     */
        seq_t bel  = bell[(size_t)(p-1)];         /* bell(p-1)                      */
        result += stir * p3 * bel;
    }

    /* Clean up */
    free(bell);
    free(pow3);
    free(prev);
    free(curr);

    return result;
}

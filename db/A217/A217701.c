/*
 * A217701  Permanent of the n X n matrix with all diagonal entries n and all
 *          off diagonal entries 1.
 * https://oeis.org/A217701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * D_{n-k} * n^k where D_m is the
 *            derangement number (A000166)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for derangement numbers.  derangements[i] = D_i. */
static seq_t *derangements = NULL;
static idx_t der_nmax = 0;          /* highest index that has been computed */

seq_t a(idx_t n)
{
    /* --- initialise or extend the derangement cache ---------- */
    if (!derangements) {                          /* first call */
        idx_t sz = n + 1;
        derangements = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!derangements) abort();

        derangements[0] = 1;                     /* D_0 = 1 */
        if (n >= 1) derangements[1] = 0;         /* D_1 = 0 */

        for (idx_t i = 2; i <= n; ++i)
            derangements[i] = (seq_t)(i-1) *
                              (derangements[i-1] + derangements[i-2]);

        der_nmax = n;
    }
    else if (der_nmax < n) {                    /* extend cache  */
        idx_t sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(derangements, sz * sizeof(seq_t));
        if (!tmp) abort();
        derangements = tmp;

        idx_t start = der_nmax + 1;
        if (start < 2) start = 2;
        for (idx_t i = start; i <= n; ++i)
            derangements[i] = (seq_t)(i-1) *
                              (derangements[i-1] + derangements[i-2]);

        der_nmax = n;
    }

    /* --- compute a(n) using the sum representation --------- */
    seq_t result = 0;
    seq_t binom  = 1;        /* C(n,0) */
    seq_t power  = 1;        /* n^0  */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t m   = n - k;                    /* index for D_{n-k} */
        seq_t term = binom * derangements[m];
        term       = term * power;
        result     += term;

        if (k < n) {                          /* update binom & power */
            binom  = binom * (seq_t)(n - k) / (seq_t)(k + 1);
            power *= (seq_t)n;
        }
    }

    return result;
}

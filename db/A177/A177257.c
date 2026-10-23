/*
 * A177257  a(n) = Sum_{j=0..n-1} (binomial(n,j) - (j+1))*A000110(j).
 * https://oeis.org/A177257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-1} (binomial(n,j) - (j+1))*A000110(j) (Bell
 *            numbers). The implementation uses an O(n) recurrence that
 *            recomputes Bell numbers only when a larger index is required. It
 *            works for any build where seq_t and idx_t are defined; seq_t may
 *            be an unsigned integer type of arbitrary preci...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*---  cache of Bell numbers -------------------------------------------*/
/* B[k] holds Bell(k).  Initially empty.  When recomputed B will contain
   values 0 .. limit. */
static seq_t *B = NULL;          /* raw array of seq_t */
static idx_t B_cap = 0;          /* number of valid entries in B */

/* Recompute Bell numbers up to 'limit' inclusive (i.e. B[0..limit]). */
static void recompute_bell(idx_t limit)
{
    /* allocate space for indices 0 .. limit : total (limit+1) elements */
    idx_t new_cap = limit + 1;
    seq_t *newB = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!newB)          /* out of memory – caller will receive 0 */
        return;

    /* free old cache (if any) */
    if (B)
        free((void *)B);

    B = newB;
    B_cap = new_cap;

    /* Bell(0) = 1 */
    B[0] = (seq_t)1;

    /* compute Bell numbers using Bell’s recurrence:
           Bell(n+1) = Σ_{k=0..n} binom(n,k) * Bell(k)    for n ≥ 0   */
    for (idx_t n = 0; n < limit; ++n) {
        seq_t sum  = (seq_t)0;
        seq_t c    = (seq_t)1;                       /* C(n,0) */
        for (idx_t k = 0; k <= n; ++k) {
            sum += c * B[k];
            if (k < n) {
                /* update to C(n,k+1) */
                c = c * (seq_t)(n - k);
                c = c / (seq_t)(k + 1);
            }
        }
        B[n + 1] = sum;             /* Bell(n+1) */
    }
}

/* Ensure that Bell numbers up to 'need' exist in the cache. */
static void ensure_bell(idx_t need)
{
    if (need < B_cap)    /* indices 0 .. B_cap-1 already present */
        return;
    recompute_bell(need);
}

/*---  main routine ----------------------------------------------*/
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* we need Bell numbers up to j = n-1 */
    if (n - (idx_t)1 >= B_cap)   /* B_cap counts elements, B[B_cap-1] last */
        ensure_bell(n - (idx_t)1);

    seq_t result = (seq_t)0;
    seq_t binom  = (seq_t)1;              /* C(n,0) */

    for (idx_t j = (idx_t)0; j < n; ++j) {
        /* term = (C(n,j) - (j+1)) * Bell(j) */
        seq_t term = binom - (seq_t)(j + (idx_t)1);
        term = term * B[j];
        result += term;

        if (j < n - (idx_t)1) {
            /* update to C(n,j+1) */
            binom = binom * (seq_t)(n - j);
            binom = binom / (seq_t)(j + (idx_t)1);
        }
    }
    return result;
}

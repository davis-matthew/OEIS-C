/*
 * A134093  Column 3 of triangle A134090.
 * https://oeis.org/A134093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coef of x^n in Σ_{k=0..n} C(n+3,k) * x^k / (1-kx)^3 /
 *            Π_{i=1..k}(1-i*x)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation evaluates the above sum directly.
   It keeps two coefficient arrays h_prev and h_curr for the series
   1 / Π_{i=1..k}(1-i*x).  The array h_k holds the coefficients of that
   series up to degree n-k.  The dynamic recurrence
       h_k[0] = 1
       h_k[m] = h_{k-1}[m] + k * h_k[m-1]   (m ≥ 1)
   is used to build h_k from h_{k-1} in O(n^2) time, which is fast
   enough for n ≤ 10 000.  The result is accumulated in a 128‑bit or
   arbitrary‑precision integer type seq_t.  No global state is required
   other than the temporary coefficient buffers. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* temporary coefficient buffers; reallocated on first use */
static seq_t *h_prev = NULL;
static seq_t *h_curr = NULL;
static size_t h_cap  = 0;

/* ensure that the coefficient buffers have space for at least 'need'
   elements; if not, reallocate larger buffers.  The old content is
   discarded (the buffers are recomputed from scratch for each call). */
static void
ensure_h_capacity(size_t need)
{
    if (need <= h_cap)
        return;
    /* free old buffers if present */
    if (h_prev) free(h_prev);
    if (h_curr) free(h_curr);
    /* allocate new buffers */
    h_prev = (seq_t *)malloc(need * sizeof(seq_t));
    h_curr = (seq_t *)malloc(need * sizeof(seq_t));
    h_cap  = need;
}

/* main sequence generator */
seq_t a(idx_t n)
{
    /* allocate coefficient buffers big enough for this n */
    /* we need indices up to n (inclusive) for h_prev and h_curr */
    ensure_h_capacity((size_t)n + 1);

    /* initialise h_prev for k = 0:
       h_0[0] = 1,  h_0[m] = 0 for m > 0 */
    for (idx_t i = 0; i <= n; ++i)
        h_prev[i] = (seq_t)0;
    h_prev[0] = (seq_t)1;

    seq_t result = (seq_t)0;                  /* a(n) to accumulate */
    seq_t binom  = (seq_t)1;                 /* C(n+3,0) */

    /* iterate over k = 0 .. n; for k > n all contributions vanish */
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t limit = n - k;                 /* t ranges 0 .. limit */
        seq_t Ak = (seq_t)0;                 /* inner sum over t */

        seq_t pow_k = (seq_t)1;              /* k^t, initial t = 0 */

        /* compute Ak = Σ_{t=0..limit} binom(t+2,2) * k^t * h_k[n-k-t] */
        for (idx_t t = 0; t <= limit; ++t)
        {
            /* coefficient of x^t in 1/(1-kx)^3  = C(t+2,2) */
            seq_t coeff = ((seq_t)(t + 2) * (t + 1)) / (seq_t)2;

            idx_t m = limit - t;            /* m = n-k-t */
            seq_t term = coeff * pow_k * h_prev[m];
            Ak += term;

            pow_k *= (seq_t)k;              /* next power of k */
        }

        result += binom * Ak;

        /* prepare h_{k+1} if we need it for the next iteration */
        if (k < n)
        {
            h_curr[0] = (seq_t)1;           /* h_{k+1}[0] = 1 */

            idx_t max_m = n - (k + 1);      /* we need h_{k+1}[m] for m ≤ max_m */
            for (idx_t m = 1; m <= max_m; ++m)
                h_curr[m] = h_prev[m] + (seq_t)(k + 1) * h_curr[m - 1];

            /* swap buffers for next loop */
            seq_t *tmp = h_prev;
            h_prev = h_curr;
            h_curr = tmp;
        }

        /* update binomial coefficient for next k:
           C(n+3,k+1) = C(n+3,k) * (n+3-k) / (k+1)  */
        binom = binom * (seq_t)(n + 3 - k) / (seq_t)(k + 1);
    }

    return result;
}

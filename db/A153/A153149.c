/*
 * A153149  Coefficients of the eighth-order mock theta function S_1(q).
 * https://oeis.org/A153149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 105, i16 n <= 325, i32 n <= 1083, i64 n <= 3841,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A153149 – Coefficients of the eight‑order mock theta function S₁(q)
 *
 *  Generating function (OEIS notation):
 *      G(q) = Sum_{m≥0} q^{m²+2m}
 *              * (1+q)(1+q³)…(1+q^{2m-1})
 *              / (1+q²)(1+q⁴)…(1+q^{2m})
 *
 *  The coefficient a(n) is the coefficient of q^n in G(q).
 *
 *  The implementation below computes all needed coefficients up to a
 *  requested index using only integer arithmetic and a small O(n²)
 *  dynamic‑programming scheme.  It keeps a static cache so that in
 *  subsequent calls the work is not repeated for indices already
 *  computed.
 *
 *  The algorithm:
 *      For each m with m²+2m ≤ nmax:
 *          let shift = m*(m+2)   // exponent of the overall q‑factor
 *          let deg   = nmax - shift   // maximal power of the remaining
 *          compute P_m(q) = product_{k=1..m} (1+q^{2k-1}) / (1+q^{2k})
 *          by starting from P_{m-1} and applying
 *              1. multiply by (1+q^{step})   where step = 2m-1
 *              2. divide by (1+q^{denom})   where denom = 2m
 *          The coefficients of P_m are then added to the final
 *          array a starting at index shift.
 *
 *  Complexity: O(nmax²) basic operations; for nmax≈10 000 all
 *  computations finish well under the 2 s limit.
 *
 *  It complies with C11 and C++17; all use only standard headers.
 */

#include <stdlib.h>
#include <stdbool.h>

/*----------------------------------------------------------------------
 *  Cache of previously computed coefficients
 *----------------------------------------------------------------------*/
static seq_t *seq_cache = NULL;   /* array a[0..N] of computed coefficients */
static idx_t  cache_cap  = 0;     /* size of seq_cache (number of elements) */

/*----------------------------------------------------------------------
 *  Zero out an existing array
 *----------------------------------------------------------------------*/
static void zero_seq_array(seq_t *arr, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        arr[i] = (seq_t)0;
}

/*----------------------------------------------------------------------
 *  Compute all coefficients up to nmax (inclusive)
 *----------------------------------------------------------------------*/
static void precompute(idx_t nmax)
{
    /* Allocate array a[0..nmax] and initialise to 0 */
    size_t cap = (size_t)(nmax + 1);
    seq_t *a = (seq_t *)calloc(cap, sizeof(seq_t));
    if (!a) { /* out of memory – abort quietly */
        if (seq_cache) free(seq_cache);
        seq_cache = NULL;
        cache_cap = 0;
        return;
    }

    /* Find maximum m with m*(m+2) ≤ nmax */
    idx_t mmax = 0;
    for (idx_t m = 0; ; ++m) {
        idx_t shift = m * (m + 2);
        if (shift > nmax) {
            mmax = m - 1;
            break;
        }
    }

    /* P_0(q) = 1, shift = 0 */
    size_t prev_size = (size_t)(nmax + 1);
    seq_t *Pprev = (seq_t *)calloc(prev_size, sizeof(seq_t));
    if (!Pprev) {
        free(a);
        return;
    }
    Pprev[0] = (seq_t)1;                /* coefficient of q^0 in P_0 */
    a[0] += Pprev[0];                   /* contribution of m = 0 */

    /* Iterate over m = 1 .. mmax */
    for (idx_t m = 1; m <= mmax; ++m) {
        idx_t shift = m * (m + 2);
        idx_t deg   = nmax - shift;         /* max degree needed for P_m */
        size_t size = (size_t)(deg + 1);

        seq_t *Q = (seq_t *)calloc(size, sizeof(seq_t));
        if (!Q) {
            free(a);
            free(Pprev);
            return;
        }

        int step   = 2 * (int)m - 1;   /* exponent in (1+q^{step}) */
        int denom  = 2 * (int)m;       /* exponent in (1+q^{denom}) */

        for (size_t d = 0; d < size; ++d) {
            seq_t val = (seq_t)0;

            /* Multiply by (1+q^{step}) */
            if (d < prev_size)
                val += Pprev[d];
            if (d >= (size_t)step && (d - (size_t)step) < prev_size)
                val += Pprev[d - (size_t)step];

            /* Divide by (1+q^{denom}) */
            if (d >= (size_t)denom)
                val -= Q[d - (size_t)denom];

            Q[d] = val;
        }

        /* Add this contribution to the final sequence */
        for (size_t d = 0; d < size; ++d) {
            size_t idx = shift + d;
            a[idx] += Q[d];
        }

        /* Prepare for next iteration */
        free(Pprev);
        Pprev     = Q;
        prev_size = size;
    }

    free(Pprev);  /* release the last Pprev buffer */

    /* Replace old cache with the newly computed array */
    if (seq_cache) free(seq_cache);
    seq_cache = a;
    cache_cap = (idx_t)cap;
}

/*----------------------------------------------------------------------
 *  Public function: a(n)
 *----------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Recompute the cache if the requested index is beyond the current range */
    if (!seq_cache || n >= cache_cap)
        precompute(n);

    return seq_cache[(size_t)n];
}

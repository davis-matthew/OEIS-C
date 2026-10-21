/*
 * A213528  E.g.f.: exp( Sum_{n>=1} Pell(n+1)*x^n/n ), where Pell(n) =
 *          A000129(n).
 * https://oeis.org/A213528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in exp( sum_{k>=1} Pell(k+1)*x^k/k )
 *            where Pell(0)=0, Pell(1)=1, Pell(k)=2*Pell(k-1)+Pell(k-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   ensure_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  sequence implementation for OEIS A213528 */
#include <stdlib.h>
#include <string.h>
#include "../utils/ensure_array.h"

/* static caches */
static seq_t *a_cache    = NULL;   /* sequence values */
static size_t a_cap     = 0, a_max = 0;

static seq_t *fact_cache = NULL;   /* fact[i] = i!   */
static size_t fact_cap  = 0, fact_max = 0;

static seq_t *pell_cache = NULL;   /* Pell numbers */
static size_t pell_cap = 0, pell_max = 0;

/* pre‑compute up to index n */
static void compute_up_to(idx_t n)
{
    size_t N = (size_t)n;
    /* allocate required space */
    ensure_array(&a_cache,    &a_cap, N + 1);
    ensure_array(&fact_cache, &fact_cap, N + 1);
    ensure_array(&pell_cache, &pell_cap, N + 3); /* need Pell up to n+2 */

    /* factorials */
    if (fact_max == 0) {
        fact_cache[0] = (seq_t)1;
        fact_max = 0;
    }
    for (idx_t i = fact_max + 1; i <= n; ++i) {
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    }
    fact_max = n;

    /* Pell numbers */
    if (pell_max == 0) {
        pell_cache[0] = (seq_t)0;
        pell_cache[1] = (seq_t)1;
        pell_max = 1;
    }
    for (idx_t i = pell_max + 1; i <= n + 2; ++i) {
        pell_cache[i] = pell_cache[i-1] * (seq_t)2 + pell_cache[i-2];
    }
    pell_max = n + 2;

    /* sequence a(n) */
    if (a_max == 0) {
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }
    if (a_max < n) {
        /* choice of arithmetic path depends on the size of seq_t */
        if (sizeof(seq_t) <= 8) {   /* 64‑bit or smaller – use 128‑bit intermediates */
            for (idx_t m = a_max + 1; m <= n; ++m) {
                unsigned __int128 sum = 0;
                unsigned __int128 prod = (unsigned __int128)fact_cache[m-1];
                for (idx_t k = 0; k <= m-1; ++k) {
                    unsigned __int128 term = (unsigned __int128)a_cache[k];
                    term *= (unsigned __int128)pell_cache[m - k + 1];
                    term *= prod;
                    sum += term;
                    if (k < m-1) prod /= (k+1);
                }
                a_cache[m] = (seq_t)sum;
            }
        } else {   /* arbitrary‑precision or 128‑bit type – normal arithmetic */
            for (idx_t m = a_max + 1; m <= n; ++m) {
                seq_t sum = (seq_t)0;
                seq_t prod = fact_cache[m-1];
                for (idx_t k = 0; k <= m-1; ++k) {
                    seq_t term = a_cache[k];
                    term = term * pell_cache[m - k + 1];
                    term = term * prod;
                    sum += term;
                    if (k < m-1) prod /= (seq_t)(k+1);
                }
                a_cache[m] = sum;
            }
        }
        a_max = n;
    }
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_cache[n];
}

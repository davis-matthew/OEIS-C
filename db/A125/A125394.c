/*
 * A125394  Number of base 31 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; for n >= 1, a(n) = trace(A^n) where A[i][j] = 1 iff
 *            |i-j| <= 5 over the digit set {0,…,30}
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_BASE 31

/* Signed addition of two non‑negative seq_t values.
   The result is stored in *res with sign *neg_res. */
static inline void signed_add(const seq_t *p1, bool neg1,
                              const seq_t *p2, bool neg2,
                              seq_t *res, bool *neg_res)
{
    if (neg1 == neg2) {
        *res = *p1 + *p2;
        *neg_res = neg1;
    } else {
        if (*p1 >= *p2) {
            *res = *p1 - *p2;
            *neg_res = neg1;
        } else {
            *res = *p2 - *p1;
            *neg_res = neg2;
        }
    }
}

/* ------------------------------------------------------------------
 *  Precomputation: traces of powers of the adjacency matrix and
 *  coefficients of the linear recurrence (order 31).
 * ------------------------------------------------------------------ */
static seq_t coeff_val[MAX_BASE + 1];
static bool   coeff_neg[MAX_BASE + 1];

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t  cache_max = 0;
static bool   initialized = false;

static void init_sequence(void)
{
    /* 1. Build adjacency matrix A. */
    seq_t A[MAX_BASE][MAX_BASE];
    for (size_t i = 0; i < MAX_BASE; ++i) {
        for (size_t j = 0; j < MAX_BASE; ++j) {
            bool cond;
            if (i > j)
                cond = (i - j) <= 5;
            else
                cond = (j - i) <= 5;
            A[i][j] = cond ? (seq_t)1 : (seq_t)0;
        }
    }

    /* 2. Compute traces a[1..31] = trace(A^k). */
    seq_t a_terms[MAX_BASE + 1];
    a_terms[0] = (seq_t)1;               /* a(0) = 1 */

    seq_t cur[MAX_BASE][MAX_BASE];
    for (size_t i = 0; i < MAX_BASE; ++i)
        for (size_t j = 0; j < MAX_BASE; ++j)
            cur[i][j] = A[i][j];

    for (size_t k = 1; k <= MAX_BASE; ++k) {
        seq_t tr = (seq_t)0;
        for (size_t i = 0; i < MAX_BASE; ++i)
            tr += cur[i][i];
        a_terms[k] = tr;

        /* cur = cur * A */
        seq_t nxt[MAX_BASE][MAX_BASE];
        for (size_t i = 0; i < MAX_BASE; ++i) {
            for (size_t j = 0; j < MAX_BASE; ++j) {
                seq_t sum = (seq_t)0;
                for (size_t m = 0; m < MAX_BASE; ++m)
                    sum += cur[i][m] * A[m][j];
                nxt[i][j] = sum;
            }
        }
        for (size_t i = 0; i < MAX_BASE; ++i)
            for (size_t j = 0; j < MAX_BASE; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* 3. Elementary symmetric sums via Newton's identities. */
    seq_t e_val[MAX_BASE + 1];
    bool   e_neg[MAX_BASE + 1];
    e_val[0] = (seq_t)1;
    e_neg[0] = false;

    for (size_t k = 1; k <= MAX_BASE; ++k) {
        seq_t sum = (seq_t)0;
        bool  sum_neg = false;

        for (size_t i = 1; i <= k; ++i) {
            seq_t term = e_val[k - i] * a_terms[i];
            bool term_neg = e_neg[k - i];
            if (i % 2 == 1) {
                signed_add(&sum, sum_neg, &term, term_neg, &sum, &sum_neg);
            } else {
                /* subtract term */
                bool term_neg_inv = !term_neg;
                signed_add(&sum, sum_neg, &term, term_neg_inv, &sum, &sum_neg);
            }
        }
        seq_t ek = sum / (seq_t)k;
        e_val[k] = ek;
        e_neg[k] = sum_neg;
    }

    /* 4. Recurrence coefficients: a(n) = Σ(-1)^{k+1} e_k a(n-k). */
    for (size_t k = 1; k <= MAX_BASE; ++k) {
        coeff_val[k] = e_val[k];
        coeff_neg[k] = e_neg[k] ^ ((k % 2) == 0);   /* flip sign on even k */
    }

    /* 5. Allocate cache and store first 32 terms. */
    cache_size = MAX_BASE + 1;
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    for (idx_t i = 0; i <= (idx_t)MAX_BASE; ++i)
        cache[i] = a_terms[i];
    cache_max = (idx_t)MAX_BASE;
    initialized = true;
}

/* ------------------------------------------------------------------
 *  Public API:  a(n)
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence();

    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* Ensure cache is large enough. */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_size = new_size;
    }

    if (n <= cache_max)
        return cache[n];

    /* Compute terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        bool  sum_neg = false;

        for (size_t k = 1; k <= MAX_BASE; ++k) {
            seq_t term = coeff_val[k] * cache[i - (idx_t)k];
            bool term_neg = coeff_neg[k];
            signed_add(&sum, sum_neg, &term, term_neg, &sum, &sum_neg);
        }
        cache[i] = sum;              /* result is non‑negative by construction */
    }
    cache_max = n;
    return cache[n];
}

/*
 * A013260  Expansion of e.g.f. tan(log(x+1) - sinh(x)).
 * https://oeis.org/A013260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in tan( log(1+x) - sinh(x) ) * a(n) is
 *            the integer such that the e.g.f. expansion is * Σ a(n) * x^n /
 *            n!
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The build supplies these typedefs:
 *   seq_t : arbitrary‑precision signed integer or native int
 *   idx_t : unsigned integer (uint64_t)
 */

static seq_t *b      = NULL;      // a(n) = b[n]
static seq_t *Ysq    = NULL;      // Ysq[i] = coefficient of x^i in (Y^2) * i!   (Ysq[0] unused)
static seq_t *U_fact = NULL;      // U_fact[k] = coefficient of x^k in U(x)   * k!
static seq_t *fact   = NULL;      // fact[k] = k!
static idx_t      cache_n = 0;     // max index previously computed (inclusive)

/* Ensure all tables are allocated and filled up to index n (inclusive). */
static void ensure_size(idx_t n)
{
    if (n < cache_n)
        return;

    /* Allocate or extend arrays. */
    size_t new_sz = (size_t)n + 1;
    b      = (seq_t*)realloc(b,      new_sz * sizeof(seq_t));
    Ysq    = (seq_t*)realloc(Ysq,    new_sz * sizeof(seq_t));
    U_fact = (seq_t*)realloc(U_fact, new_sz * sizeof(seq_t));
    fact   = (seq_t*)realloc(fact,   new_sz * sizeof(seq_t));

    /* Base definitions for n == 0. */
    if (cache_n == 0) {
        b[0]      = (seq_t)0;
        Ysq[0]    = (seq_t)0;
        U_fact[0] = (seq_t)0;
        fact[0]   = (seq_t)1;
    }

    /* Fill factorials from cache_n+1 up to n. */
    for (idx_t i = cache_n + 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Fill U_fact from max(1, cache_n+1) up to n. */
    idx_t startUF = (cache_n + 1 > 0) ? cache_n + 1 : (idx_t)1;
    for (idx_t k = startUF; k <= n; ++k) {
        seq_t term = fact[k - 1];                    // (k-1)!
        seq_t sign = ((k - 1) & 1) ? (seq_t)-1 : (seq_t)1; /* (-1)^(k-1) */
        U_fact[k] = term * sign;
        if (k & 1)                                    /* subtract 1 for odd k */
            U_fact[k] -= (seq_t)1;
    }

    /* Compute b and Ysq for new indices. */
    for (idx_t k = cache_n; k < n; ++k) {
        /* Compute b[k+1] */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;
        for (idx_t i = 0; i <= k; ++i) {
            seq_t Sfi = (i == 0) ? (seq_t)1 : Ysq[i];        /* S_i = 1 + Y^2 fact coeff at i */
            seq_t term = Sfi * U_fact[k - i + 1] * binom;
            sum += term;
            if (i < k) {
                binom = binom * (seq_t)(k - i) / (seq_t)(i + 1);
            }
        }
        b[k + 1] = sum;

        /* Compute Ysq[k+1] = coefficient of x^(k+1) in (Y^2) * (k+1)! */
        idx_t m      = k + 1;
        seq_t Ysq_val = (seq_t)0;
        seq_t binom2  = (seq_t)1;
        for (idx_t i = 0; i <= m; ++i) {
            seq_t term2 = b[i] * b[m - i] * binom2;
            Ysq_val += term2;
            if (i < m) {
                binom2 = binom2 * (seq_t)(m - i) / (seq_t)(i + 1);
            }
        }
        Ysq[m] = Ysq_val;
    }

    cache_n = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    ensure_size(n);
    return b[n];
}

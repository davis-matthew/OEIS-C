/*
 * A202275  Differences between A074753 (number of integers k such that
 *          sigma(k) <= n) and A202276 (number of integers k <= n such that
 *          sigma(x) = k has no solution); sigma = A000203.
 * https://oeis.org/A202275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A074753(n) – A202276(n) A074753(n): number of integers k
 *            with σ(k) ≤ n A202276(n): number of integers k ≤ n such that
 *            there is no integer x with σ(x) = k The function precomputes
 *            σ(k) for all k ≤ current n and tracks which values σ(k) take, to
 *            answer queries in O(n) time per call (using me...
 * Range:     i8 n <= 59, i16 n <= 9999, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Global state ------------------------------------------------------- */
static idx_t      _max_n  = 0;      /* largest n for which σ is known       */
static seq_t     *_sigma  = NULL;   /* σ(k) for 1 ≤ k ≤ _max_n              */
static bool      *_seen   = NULL;   /* true if some σ(x)=k for k ≤ _max_n   */

/* Forward declaration */
static void __ensure_precompute(idx_t n);

/* -------------------------------------------------------------------- */
/*  Compute σ(k) for all k ≤ n and build the array of attainable values. */
static void __ensure_precompute(idx_t n)
{
    if (n <= _max_n)                     /* nothing new to compute       */
        return;

    /* Allocate new tables -------------------------------------------- */
    seq_t *new_sigma = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    bool  *new_seen  = (bool *)calloc((n + 1), sizeof(bool));
    if (!new_sigma || !new_seen) {        /* allocation failed           */
        /* In a real build we would handle the error; here we assume success */
    }

    /* Initialise σ(k) to 0 ------------------------------------------ */
    for (idx_t i = 1; i <= n; ++i)
        new_sigma[i] = 0;

    /* Sieve to compute divisor sums --------------------------------- */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            new_sigma[j] += (seq_t)i;

    /* Mark attainable σ-values -------------------------------------- */
    for (idx_t x = 1; x <= n; ++x) {
        seq_t s = new_sigma[x];
        if (s <= (seq_t)n)
            new_seen[(idx_t)s] = true;
    }

    /* Replace old tables */
    free(_sigma);
    free(_seen);
    _sigma  = new_sigma;
    _seen   = new_seen;
    _max_n  = n;
}

/* -------------------------------------------------------------------- */
/*  Sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Make sure data up to n is available */
    __ensure_precompute(n);

    /* Count of k with σ(k) ≤ n (k runs over all positive integers) */
    seq_t countA074 = 0;
    for (idx_t k = 1; k <= n; ++k)
        if (_sigma[k] <= (seq_t)n)
            ++countA074;

    /* Count of k ≤ n for which σ(x) = k has no solution */
    seq_t countUnatt = 0;
    for (idx_t k = 1; k <= n; ++k)
        if (!_seen[k])
            ++countUnatt;

    return countA074 - countUnatt;
}

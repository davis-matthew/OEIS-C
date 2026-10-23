/*
 * A024417  s(n,a(n)) = max{s(n,k): k=1,2,...,n}, n >= 1, where s(n,k) =
 *          Stirling numbers of the second kind.
 * https://oeis.org/A024417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   logsumexp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming using the recurrence
   S(n,k) = k*S(n‑1,k) + S(n‑1,k‑1).
   For each n the index k with the maximal S(n,k) is stored as a_cache[n]. */
/* Note: no closed‑form formula is known; this algorithm uses O(n²) time and
   O(n) space, which is fast enough for n up to about 10 000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/logsumexp.h"

static double *prev_log = NULL;  /* log values of S(prev_n,k)  */
static double *curr_log = NULL;  /* temporary log values for the next row */
static size_t log_cap   = 0;     /* current capacity of the log arrays */

static idx_t *a_cache = NULL;   /* cache of computed a(n) values */
static size_t a_cap   = 0;

static idx_t max_cached_n = 0;

/* Ensure log arrays are large enough */
static void ensure_log_cap(size_t need)
{
    if (log_cap >= need) return;
    double *new_prev = (double*)realloc(prev_log, need * sizeof(double));
    double *new_curr = (double*)realloc(curr_log, need * sizeof(double));
    if (!new_prev || !new_curr) abort();
    prev_log = new_prev;
    curr_log = new_curr;
    log_cap  = need;
}

/* Ensure a_cache is large enough */
static void ensure_a_cap(size_t need)
{
    if (a_cap >= need) return;
    idx_t *new_a = (idx_t*)realloc(a_cache, need * sizeof(idx_t));
    if (!new_a) abort();
    a_cache = new_a;
    a_cap   = need;
}

/* Compute a(n) = max_k S(n,k) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* First call: initialize tables for n = 1 */
    if (prev_log == NULL) {
        ensure_log_cap(3);              /* indices 0..2 at least */
        prev_log[0] = -INFINITY;        /* log(0) */
        prev_log[1] = 0.0;              /* log(1) = 0 */
        ensure_a_cap(2);
        a_cache[1] = 1;
        max_cached_n = 1;
    }

    /* Extend cache up to requested n if needed */
    if (n > max_cached_n) {
        for (idx_t i = max_cached_n + 1; i <= n; ++i) {
            size_t need = (size_t)i + 1;   /* indices 0..i */
            ensure_log_cap(need);

            double max_log = -INFINITY;
            size_t  max_k   = 1;

            for (size_t k = 1; k <= (size_t)i; ++k) {
                double log_a = -INFINITY;
                if (k <= (size_t)i - 1) { /* S(i-1,k) exists */
                    log_a = log((double)k) + prev_log[k];
                }
                double log_b = -INFINITY;
                if (k - 1 <= (size_t)i - 1) { /* S(i-1,k-1) exists */
                    log_b = prev_log[k - 1];
                }
                curr_log[k] = logsumexp(log_a, log_b);

                if (curr_log[k] > max_log) {
                    max_log = curr_log[k];
                    max_k   = k;
                }
            }

            /* Prepare for next iteration */
            double *tmp   = prev_log;
            prev_log      = curr_log;
            curr_log      = tmp;

            ensure_a_cap((size_t)i + 1);
            a_cache[i] = (idx_t)max_k;
        }
        max_cached_n = n;
    }

    return (seq_t)a_cache[n];
}

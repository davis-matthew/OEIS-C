/*
 * A081827  Successive differences of Hofstadter Q-sequence.
 * https://oeis.org/A081827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Q(n+1) - Q(n)
 * Range:     i8 n <= 766, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Q is the Hofstadter Q-sequence:
 *   Q(1) = Q(2) = 1;
 *   for n > 2: Q(n) = Q(n - Q(n-1)) + Q(n - Q(n-2));
 * The function caches intermediate values of Q to allow
 * arbitrary order queries up to any n requested.
 */

#include <stdlib.h>

static seq_t *Q_cache = NULL;    /* 1‑based array of Q values                               */
static idx_t   Q_alloc = 0;      /* total allocated length             */
static idx_t   Q_limit = 0;      /* largest n computed so far (inclusive) */

/* Ensure the cache contains Q(i) for all i <= n_needed */
static void ensure_Q(idx_t n_needed)
{
    if (n_needed <= Q_limit)
        return;

    /* Allocate or grow the cache to accommodate up to n_needed */
    if (Q_alloc < n_needed + 2) {            /* +2 to have index 1..n_needed */
        idx_t new_alloc = n_needed + 2;
        Q_cache = (seq_t *)realloc(Q_cache, sizeof(seq_t) * new_alloc);
        if (!Q_cache) {
            /* Out of memory – aborting for simplicity */
            exit(1);
        }
        Q_alloc = new_alloc;
    }

    /* Compute any missing Q values */
    for (idx_t k = Q_limit + 1; k <= n_needed; ++k) {
        if (k <= 2) {
            Q_cache[k] = (seq_t)1;
        } else {
            seq_t q1 = Q_cache[k - (idx_t)Q_cache[k - 1]];
            seq_t q2 = Q_cache[k - (idx_t)Q_cache[k - 2]];
            Q_cache[k] = q1 + q2;
        }
    }
    Q_limit = n_needed;
}

/* Return the nth term of OEIS A081827 */
seq_t a(idx_t n)
{
    /* We need Q(n) and Q(n+1) */
    ensure_Q(n + 1);
    return Q_cache[n + 1] - Q_cache[n];
}

/*
 * A150370  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1, 1,
 *          0), (1, 0, -1), (1, 0, 0), (1, 0, 1)}.
 * https://oeis.org/A150370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Σ_{d=0}^{⌊n/2⌋} binom(n,d)·(n‑2d+1)/(n‑d+1) ·
 *            binom(d,⌊d/2⌋) · M_{\,n‑d} where M_k is the number of walks of
 *            length k with steps {-1,0,+1} that never go below 0 (the Motzkin
 *            meander numbers)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

/* ---------- cache for Motzkin meander numbers M_k = Q[k] ---------- */
static seq_t *Q = NULL;          /* Q[0..size-1] = M_k                 */
static size_t Qsize = 0;         /* current allocated size              */
static seq_t *prev = NULL;       /* array of size Qsize-1 for DP step   */
static size_t prev_len = 0;      /* length of prev array                */

/* Precompute M_k up to k = n (inclusive).  The values are stored in Q.   */
static void ensure_M(size_t n)
{
    if (Qsize > n) return;

    size_t newsize = n + 1;
    seq_t *tmp = (seq_t *)realloc(Q, newsize * sizeof(seq_t));
    if (!tmp) return;          /* allocation failure, leave function */
    Q = tmp;

    for (size_t k = Qsize; k <= n; ++k) {
        if (k == 0) {          /* M_0 = 1 ; initialise DP base    */
            Q[0] = (seq_t)1;
            prev = (seq_t *)malloc(1 * sizeof(seq_t));
            prev[0] = Q[0];
            prev_len = 1;
            continue;
        }

        /* allocate current array of size k+1 */
        seq_t *curr = (seq_t *)malloc((k + 1) * sizeof(seq_t));

        /* h = 0  : from prev[0] via 0-step or from prev[1] via -1 step */
        curr[0] = prev[0];
        if (prev_len > 1) curr[0] += prev[1];

        /* middle heights */
        for (size_t h = 1; h < k; ++h) {
            seq_t val = prev[h];
            val += prev[h - 1];                      /* step +1  */
            if (h + 1 < prev_len) val += prev[h + 1]; /* step -1  */
            curr[h] = val;
        }

        /* highest height k : only possible from prev[k‑1] via +1 */
        curr[k] = prev[k - 1];

        /* total number of meanders of length k */
        seq_t sum = 0;
        for (size_t h = 0; h <= k; ++h) sum += curr[h];
        Q[k] = sum;

        free(prev);                    /* discard previous step array */
        prev = curr;
        prev_len = k + 1;
    }

    Qsize = n + 1;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    ensure_M(N);                    /* Q[0..N] now valid */
    seq_t result = 0;

    for (size_t d = 0; d <= N / 2; ++d) {
        size_t i  = N - d;          /* number of +1 steps           */
        size_t r  = N - 2 * d;      /* i – d                         */

        seq_t binom_nd = binom((idx_t)N, (idx_t)d);
        seq_t B = (binom_nd * (seq_t)(r + 1)) / (seq_t)(i + 1); /* ballot */

        seq_t P = binom((idx_t)d, (idx_t)(d / 2));              /* y‑walks */
        seq_t Qi = Q[i];                                       /* z‑walks */

        seq_t term = B;
        term *= P;
        term *= Qi;
        result += term;
    }
    return result;
}

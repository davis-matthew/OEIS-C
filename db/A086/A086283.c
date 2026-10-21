/*
 * A086283  a(1)=1, a(2)=1 and, for n>2, a(n) is the smallest positive integer
 *          such that the sequence of second order absolute difference is the
 *          sequence of positive integers {1,2,3,4,...}.
 * https://oeis.org/A086283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086283 – Minimal a(n) making the second order absolute difference equal to 1,2,3,...
   a(1) = 1, a(2) = 1.
   For n > 2, a(n) is the smallest positive integer x such that
          | |x – a(n‑1)| – |a(n‑1) – a(n‑2)| | = n‑2.   */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0, but safe */

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Ensure cache exists and has enough space */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed */
    }
    if (n > cap) {
        size_t newcap = cap;
        while (newcap < n) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* Compute terms up to n if missing */
    for (idx_t i = len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;
        } else if (i == 2) {
            cache[1] = (seq_t)1;
        } else {
            seq_t a_prev     = cache[i - 2];      /* a(i-1) */
            seq_t a_prevprev = cache[i - 3];      /* a(i-2) */
            /* b = |a_prev - a_prevprev| */
            seq_t b = (a_prev > a_prevprev) ? (a_prev - a_prevprev) : (a_prevprev - a_prev);
            seq_t target = (seq_t)(i - 2);

            /* Possible |x - a_prev| values */
            seq_t y1 = b + target;                 /* b + target   */
            seq_t y2 = 0;                          /* will be set only if valid */
            int y2_valid = 0;
            if (b >= target) {
                y2 = b - target;                   /* b - target   */
                y2_valid = 1;
            }

            seq_t best = 0; /* 0 means unset */

            /* Helper to try a candidate x */
            #define TRY_CANDIDATE(x) do {        \
                if ((x) >= 1) {                  \
                    if (best == 0 || (x) < best) best = (x); \
                }                                \
            } while (0)

            /* +y1 */
            TRY_CANDIDATE(a_prev + y1);
            /* -y1 */
            if (a_prev >= y1) TRY_CANDIDATE(a_prev - y1);

            if (y2_valid) {
                /* +y2 */
                TRY_CANDIDATE(a_prev + y2);
                /* -y2 */
                if (a_prev >= y2) TRY_CANDIDATE(a_prev - y2);
            }

            cache[i - 1] = best;
            #undef TRY_CANDIDATE
        }
    }

    len = n;
    return cache[n - 1];
}

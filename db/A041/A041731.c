/*
 * A041731  Denominators of continued fraction convergents to sqrt(385).
 * https://oeis.org/A041731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, * a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 5, a(5) = 8,
 *            a(6) = 29, a(7) = 37, * a(8) = 103, a(9) = 140, a(10) = 523,
 *            a(11) = 663, a(12) = 1186, a(13) = 1849, * a(14) = 3035, a(15) =
 *            4884, a(16) = 188627, ... * For n > 31: a(n) = 191662 * a(n-16)
 *            - a(n-32) (recurrence)
 * Range:     u8 none, u16 n <= 15, u32 n <= 31, u64 n <= 61, u128 n <= 118,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 174 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static void compute_base_terms(seq_t *cache) {
    const seq_t S = (seq_t)385;
    seq_t m = 0;
    seq_t d = 1;
    seq_t aPrev = (seq_t)19;             /* a0 */
    seq_t q_prevprev = 0;                /* q_{-1} */
    seq_t q_prev = 1;                    /* q_0 = 1 */

    cache[0] = q_prev;                   /* a(0) */

    for (idx_t i = 1; i < 32; ++i) {
        seq_t mNew = d * aPrev - m;
        seq_t dNew = (S - mNew * mNew) / d;
        seq_t aCur = ((seq_t)19 + mNew) / dNew;
        seq_t qCur = aCur * q_prev + q_prevprev;

        cache[i] = qCur;

        /* shift for next step */
        m = mNew;
        d = dNew;
        aPrev = aCur;
        q_prevprev = q_prev;
        q_prev = qCur;
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;  /* last index computed */

    /* Allocate initial cache */
    if (cache_size == 0) {
        cache_size = (n + 1 > 32 ? n + 1 : 32);
        cache = mem_new(seq_t, cache_size);
        if (!cache) return 0;          /* allocation failure – not expected */
        compute_base_terms(cache);
        computed = 31;                 /* base terms up to index 31 are ready */
    }

    /* Grow cache if needed for the current n */
    if (n + 1 > cache_size) {
        cache = mem_resize(seq_t, cache, (n + 1));
        if (!cache) return 0;          /* allocation failure – not expected */
        cache_size = n + 1;
    }

    /* Compute missing terms using recurrence, if any */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* recurrence: a(i) = 191662*a(i-16) - a(i-32)   for i>31 */
            cache[i] = (seq_t)191662 * cache[i - 16] - cache[i - 32];
        }
        computed = n;
    }

    return cache[n];
}

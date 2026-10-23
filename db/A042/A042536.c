/*
 * A042536  Numerators of continued fraction convergents to sqrt(797).
 * https://oeis.org/A042536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 11, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042536: numerators of convergents to sqrt(797).
   Recurrence: p_-2 = 0, p_-1 = 1;
   for k ≥ 0:  p_k = a_k * p_{k-1} + p_{k-2}
   with a_0 = ⌊√797⌋, a_k (k ≥ 1) the partial quotients of the continued fraction of √797.
   a(0) = 28, a(1) = 113, … */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *periodic = NULL;
static size_t period_len = 0;
static seq_t a0;

static void init_period(void) {
    if (periodic) return;

    /* compute a0 = floor(sqrt(797)) */
    seq_t tmp = floor_sqrt((seq_t)797);
    uint64_t a0_small = (uint64_t)tmp;   /* fits in uint64_t */
    a0 = (seq_t)a0_small;

    /* continued‑fraction expansion of √797 for the periodic part */
    uint64_t D = 797;
    uint64_t m = 0, d = 1, a_cur = a0_small;
    uint64_t terms_buf[128];
    size_t len = 0;

    do {
        m = d * a_cur - m;
        d = (D - m * m) / d;
        a_cur = (a0_small + m) / d;
        terms_buf[len++] = a_cur;
    } while (a_cur != 2 * a0_small); /* period ends when a_k == 2*a0 */

    period_len = len;
    periodic = (seq_t *)malloc(period_len * sizeof(seq_t));
    for (size_t i = 0; i < period_len; ++i)
        periodic[i] = (seq_t)terms_buf[i];
}

seq_t a(idx_t n) {
    init_period();

    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t p_cur = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_cur;
        if (i == 0)
            a_cur = a0;
        else
            a_cur = periodic[((size_t)(i - 1)) % period_len];

        p_cur = a_cur * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }
    return p_cur;
}

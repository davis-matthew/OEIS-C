/*
 * A042837  Denominators of continued fraction convergents to sqrt(949).
 * https://oeis.org/A042837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent of √949. Recurrence:
 *            a(0)=1; for n≥1, a(n) = a(n‑1) * b_n + a(n‑2), where b_n are the
 *            continued‑fraction coefficients of √949
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Build the period of the continued fraction for √949
   and store it in *period (dynamic array). */
static void init_period(seq_t **period, size_t *len) {
    const uint64_t D = 949ULL;
    /* integer sqrt of D */
    uint64_t a0 = 0;
    for (uint64_t t = 1; t * t <= D; ++t) a0 = t;

    uint64_t m = 0, d = 1, a = a0;
    uint64_t *tmp = NULL;
    size_t cap = 8, count = 0;
    tmp = (uint64_t *)malloc(cap * sizeof(uint64_t));

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;          /* next coefficient */
        if (count == cap) {
            cap *= 2;
            uint64_t *newtmp = (uint64_t *)realloc(tmp, cap * sizeof(uint64_t));
            if (!newtmp) { free(tmp); exit(1); }
            tmp = newtmp;
        }
        tmp[count++] = a;
        if (a == 2 * a0) break;
    }

    *len = count;
    *period = (seq_t *)malloc(count * sizeof(seq_t));
    for (size_t i = 0; i < count; ++i) (*period)[i] = (seq_t)tmp[i];
    free(tmp);
}

seq_t a(idx_t n) {
    /* static cache for continued‑fraction coefficients */
    static seq_t *period = NULL;
    static size_t period_len = 0;
    static seq_t S_a0 = 0;                 /* a0 as seq_t */

    if (period == NULL) {
        const uint64_t D = 949ULL;
        uint64_t a0 = 0;
        for (uint64_t t = 1; t * t <= D; ++t) a0 = t;
        S_a0 = (seq_t)a0;
        init_period(&period, &period_len);
    }

    seq_t q_prevprev = 1; /* q_{-2} */
    seq_t q_prev = 0;     /* q_{-1} */
    seq_t q_curr = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = S_a0;
        } else {
            size_t pos = (size_t)((k - 1) % period_len);
            a_k = period[pos];
        }
        q_curr = a_k * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }
    return q_prev;
}

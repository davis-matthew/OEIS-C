/*
 * A134078  Expansion of (phi(-q) / phi(-q^2))^3 * phi(q^3)^5 / phi(-q^6) in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A134078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Euler transform of the period‑12 sequence * s = [-6,
 *            3, 4, 0, -6, -10, -6, 0, 4, 3, -6, -4]. * Recurrence: * b[0] =
 *            1, * for m ≥ 1: * b[m] = (1/m) * Σ_{k=1}^{m} S(k) * b[m–k], *
 *            where S(k) = Σ_{d|k} d * s[d]. * This implements A134078
 * Range:     i8 n <= 3, i16 n <= 90, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

static const int base_seq[12] = { -6, 3, 4, 0, -6, -10, -6, 0, 4, 3, -6, -4 };

/* cached values */
static seq_t *b   = NULL;    /* Euler transform terms */
static size_t  b_cap = 0;
static idx_t   b_size = 0;

static seq_t *S   = NULL;    /* divisor‑sum terms */
static size_t  S_cap = 0;
static idx_t   S_size = 0;

seq_t a(idx_t n)
{
    /* ensure enough space */
    size_t needed = (size_t)n + 1;
    if (b_cap < needed) {
        seq_t *newb = seq_realloc(b, b_cap, needed);
        b = newb;
        b_cap = needed;
    }
    if (S_cap < needed) {
        seq_t *newS = seq_realloc(S, S_cap, needed);
        S = newS;
        S_cap = needed;
    }

    /* initialise the first term */
    if (b_size == 0) {
        b[0] = (seq_t)1;   /* b[0] = 1 */
        b_size = 1;
    }

    /* if already computed, return */
    if (n < b_size)
        return b[n];

    /* compute S(k) for k = S_size .. n */
    for (idx_t k = S_size; k <= n; ++k) {
        seq_t sumDiv = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                sumDiv += (seq_t)d * (seq_t)base_seq[(d - 1) % 12];
                idx_t d2 = k / d;
                if (d2 != d)
                    sumDiv += (seq_t)d2 * (seq_t)base_seq[(d2 - 1) % 12];
            }
        }
        S[k] = sumDiv;
    }
    S_size = n + 1;

    /* compute the Euler transform terms up to n */
    for (idx_t m = b_size; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k)
            sum += S[k] * b[m - k];
        b[m] = sum / (seq_t)m;
    }
    b_size = n + 1;

    return b[n];
}

/*
 * A014814  Inverse of 805th cyclotomic polynomial.
 * https://oeis.org/A014814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in 1/Φ_805(x); 1/Φ_805(x) =
 *            ∏_{d∈{5,7,23,805}} (1‑x^d)^(–1) · ∏_{d∈{1,35,115,161,805}}
 *            (1‑x^d)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const size_t PERIOD = 805;

/* Cached period values: will be filled on the first call. */
static seq_t *period = NULL;
static int initialized = 0;

static void init_period(void)
{
    if (initialized)
        return;
    initialized = 1;

    seq_t *cur  = (seq_t*)malloc(sizeof(seq_t) * PERIOD);
    seq_t *next = (seq_t*)malloc(sizeof(seq_t) * PERIOD);
    if (!cur || !next)
        return;                         /* allocation failed */

    /* start with constant term 1 and all other coefficients 0. */
    for (size_t i = 0; i < PERIOD; ++i)
        cur[i] = 0;
    cur[0] = 1;

    /* multiply by (1 – x^d)^(–1) for d = 5,7,23,805 */
    const size_t inv_ds[] = {5, 7, 23, 805};
    const size_t inv_cnt  = sizeof(inv_ds) / sizeof(inv_ds[0]);

    for (size_t t = 0; t < inv_cnt; ++t) {
        size_t d = inv_ds[t];
        for (size_t n = 0; n < PERIOD; ++n) {
            next[n] = cur[n];
            if (n >= d)
                next[n] += next[n - d];
        }
        for (size_t n = 0; n < PERIOD; ++n)
            cur[n] = next[n];
    }

    /* multiply by (1 – x^d) for d = 1,35,115,161,805 */
    const size_t num_ds[] = {1, 35, 115, 161, 805};
    const size_t num_cnt  = sizeof(num_ds) / sizeof(num_ds[0]);

    for (size_t t = 0; t < num_cnt; ++t) {
        size_t d = num_ds[t];
        for (size_t n = 0; n < PERIOD; ++n) {
            next[n] = cur[n];
            if (n >= d)
                next[n] -= cur[n - d];
        }
        for (size_t n = 0; n < PERIOD; ++n)
            cur[n] = next[n];
    }

    /* store the computed period */
    period = (seq_t*)malloc(sizeof(seq_t) * PERIOD);
    if (!period) {
        free(cur);
        free(next);
        return;
    }
    for (size_t i = 0; i < PERIOD; ++i)
        period[i] = cur[i];

    free(cur);
    free(next);
}

seq_t a(idx_t n)
{
    init_period();
    return period[(size_t)(n % PERIOD)];
}

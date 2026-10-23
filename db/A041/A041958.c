/*
 * A041958  Numerators of continued fraction convergents to sqrt(502).
 * https://oeis.org/A041958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = (convergents of sqrt(502)) numerator, a(0) = 22.
 *            Computation uses continued‑fraction recurrence: p_−2 = 0, p_−1 =
 *            1 p_n = a_n * p_{n-1} + p_{n-2} (n ≥ 0) The partial quotients
 *            a_n for sqrt(502) are generated iteratively (classical
 *            continued‑fraction algorithm). All work is performed in seq...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cached numerators               */
static idx_t cache_size = 0;     /* number of terms stored          */

/* Continued‑fraction state for sqrt(502) */
static uint64_t cf_m = 0;        /* m_k          */
static uint64_t cf_d = 1;        /* d_k          */
static uint64_t cf_a = 22;       /* a_k (a_0 = 22) */

seq_t a(idx_t n)
{
    /* Grow the cache to include a(n) */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* First term already known */
        if (cache_size == 0) {
            cache[0] = (seq_t)cf_a;   /* a(0) = 22 */
            cache_size = 1;
        }

        /* Compute subsequent terms in order */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* generate next partial quotient a_i */
            cf_m = cf_d * cf_a - cf_m;
            cf_d = (502ULL - cf_m * cf_m) / cf_d;
            cf_a = (22 + cf_m) / cf_d;

            seq_t p_prev1 = cache[i - 1];
            seq_t p_prev2 = (i >= 2) ? cache[i - 2] : (seq_t)1; /* p_{-1} = 1 */

            /* p_i = a_i * p_{i-1} + p_{i-2} */
            cache[i] = (seq_t)cf_a * p_prev1 + p_prev2;
        }
        cache_size = new_size;
    }
    return cache[n];
}

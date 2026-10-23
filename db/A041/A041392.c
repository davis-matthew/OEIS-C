/*
 * A041392  Numerators of continued fraction convergents to sqrt(211).
 * https://oeis.org/A041392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            expansion of √211. Recurrence: Let D = 211, a0 = ⌊√D⌋ = 14.
 *            Initialize m0 = 0, d0 = 1, a0 = ⌊√D⌋. For k ≥ 0: m_{k+1} = d_k *
 *            a_k − m_k d_{k+1} = (D − m_{k+1}²) / d_k a_{k+1} = (a0 +
 *            m_{k+1}) / d_{k+1} (integer division) Convergents:...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 21, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t SQRT_D = (seq_t)14;   /* ⌊√211⌋ */
static const seq_t D_SEQ  = (seq_t)211;  /* 211 as seq_t */

static seq_t *cache   = NULL;          /* stored p_k values   */
static size_t cache_len = 0;           /* number of stored terms */
static size_t cache_cap = 0;           /* allocated capacity   */

/* state of the continued fraction for the next term */
static seq_t cf_m = 0;
static seq_t cf_d = 1;
static seq_t cf_a = SQRT_D;   /* a0 */

seq_t a(idx_t n)
{
    /* allocate at least n+1 elements */
    if (cache_len == 0) {
        /* first term p0 = a0 */
        cache_cap = (n + 1 < 16) ? 16 : (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* memory failure */

        /* store first numerator */
        cache[0] = cf_a;   /* p0 = a0 */
        cache_len = 1;

        /* initialise continued fraction for k=0 */
        cf_m = 0;
        cf_d = 1;
        cf_a = SQRT_D;
    } else if (n >= cache_len) {
        /* expand the cache if needed */
        if (n + 1 > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)(n + 1))
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* memory failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute terms until we reach n */
        while (cache_len <= (size_t)n) {
            /* compute next continued-fraction coefficient */
            seq_t m_next = cf_d * cf_a - cf_m;
            seq_t d_next = (D_SEQ - m_next * m_next) / cf_d;
            seq_t a_next = (SQRT_D + m_next) / d_next;

            /* compute next convergent numerator */
            seq_t prev1 = cache[cache_len - 1];
            seq_t prev2 = (cache_len >= 2) ? cache[cache_len - 2] : (seq_t)1;
            seq_t p_next = a_next * prev1 + prev2;

            /* store it */
            cache[cache_len] = p_next;
            cache_len++;

            /* advance continued-fraction state */
            cf_m = m_next;
            cf_d = d_next;
            cf_a = a_next;
        }
    }

    return cache[n];
}

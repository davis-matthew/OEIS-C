/*
 * A041885  Denominators of continued fraction convergents to sqrt(464).
 * https://oeis.org/A041885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041885 – denominators of continued fraction convergents to sqrt(464)
   Recurrence:  a(n) = 19602*a(n-10) - a(n-20)   for n > 19
   Initial values (n = 0 … 19) are obtained from the convergents of
   the continued fraction of sqrt(464) and are not hard‑coded. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* number of valid entries computed */
static size_t cache_cap = 0;         /* allocated capacity (bytes) */
static bool init_done = false;       /* first‑time initialization flag */

/* ------------------------- helper: resize cache if needed ------------------- */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);   /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------- compute first 20 denominators ----------------- */
static void init_first_twenty(void)
{
    const idx_t D  = 464;              /* number under the square root   */
    const idx_t a0 = 21;               /* floor(sqrt(464))              */

    idx_t m = 0, d = 1, a = a0;        /* CF variables (indices)        */

    seq_t p_prev2 = 0, p_prev1 = 1;    /* p[-2], p[-1] for convergents   */
    seq_t q_prev2 = 1, q_prev1 = 0;    /* q[-2], q[-1] for convergents   */

    for (idx_t i = 0; i < 20; ++i) {
        if (i > 0) {
            /* next continued fraction term */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
        }

        /* convergents */
        seq_t p = (seq_t)a * p_prev1 + p_prev2;
        seq_t q = (seq_t)a * q_prev1 + q_prev2;

        cache[(size_t)i] = q;  /* store a(i) */

        /* shift for next step */
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    cache_len = 20;
}

/* ---------------------------- main sequence function --------------------- */
seq_t a(idx_t n)
{
    if (!init_done) {
        ensure_cache_cap(20);
        init_first_twenty();
        init_done = true;
    } else if (n + 1 > cache_cap) {
        ensure_cache_cap(n + 1);  /* enlarge capacity if needed */
    }

    /* if already computed, just return */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* compute terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[(size_t)i] =
            ((seq_t)19602 * cache[(size_t)(i - 10)] - cache[(size_t)(i - 20)]);
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}

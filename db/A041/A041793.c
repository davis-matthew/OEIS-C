/*
 * A041793  Denominators of continued fraction convergents to sqrt(417).
 * https://oeis.org/A041793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 170645294*a(n-18) - a(n-36) for n > 35 Initial values
 *            a(0)..a(35) are the denominators of the continued‑fraction
 *            convergents to sqrt(417); they are generated automatically from
 *            the period of sqrt(417) (period length = 18)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of already computed terms */
static seq_t *cache       = NULL;
static idx_t cache_size   = 0;   /* number of terms stored (cache[0]..cache[cache_size-1]) */
static idx_t cache_capacity = 0; /* allocated size */

/* compute the first 36 terms from the continued fraction of sqrt(417) */
static void init_cache(void)
{
    if (cache != NULL)
        return;                 /* already initialized */

    const idx_t N   = 417;
    const idx_t a0  = 20;        /* floor(sqrt(417)) */
    idx_t m = 0, d = 1, a = a0;

    /* store partial quotients after a0 until the period ends
       (period ends when a == 2*a0) */
    idx_t period[200];
    idx_t period_len = 0;
    do {
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;
        period[period_len++] = a;
    } while (a != 2 * a0);      /* period completed; length = period_len */

    /* allocate space for 36 values (indices 0..35) */
    cache_capacity = 36;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    cache_size  = 36;

    /* compute convergents: p_{-2}=0, p_{-1}=1; q_{-2}=1, q_{-1}=0 */
    seq_t p_prevprev = 0, p_prev = 1;
    seq_t q_prevprev = 1, q_prev = 0;

    for (idx_t k = 0; k < 36; ++k) {
        seq_t ak = (k == 0) ? a0 : period[(k - 1) % period_len];
        seq_t q  = ak * q_prev + q_prevprev;   /* denominator of convergent k */
        cache[k] = q;

        /* update p/q for next step (p is not needed afterwards) */
        seq_t p = ak * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev     = p;

        q_prevprev = q_prev;
        q_prev     = q;
    }
}

/* extend the cache up to index n (inclusive) using the recurrence */
static void extend_cache(idx_t n)
{
    if (n < cache_size)
        return;                      /* nothing to do */

    /* enlarge allocation if necessary */
    if (n + 1 > cache_capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_capacity = new_cap;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)170645294 * cache[i - 18] - cache[i - 36];
    }
    cache_size = n + 1;
}

/* public function: returns a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    init_cache();
    extend_cache(n);
    return cache[n];
}

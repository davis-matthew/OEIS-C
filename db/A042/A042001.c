/*
 * A042001  Denominators of continued fraction convergents to sqrt(523).
 * https://oeis.org/A042001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 163620601252 * a(n-22) – a(n-44) (n > 43)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 21, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The first 44 terms (n = 0 … 43) are obtained by the continued‑fraction
   convergents of √523.  Subsequent terms follow the above recurrence. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Memoisation storage */
static seq_t *cache = NULL;              /* array of previously computed terms */
static size_t cache_size = 0;            /* capacity of cache */
static size_t last_computed = 0;         /* largest index already computed */

/* Continued fraction state for sqrt(523); valid only while cf_index <= 43 */
static int cf_initialized = 0;
static uint64_t cf_m, cf_d; /* current m and d */
static int cf_a;            /* current coefficient a */
static int cf_index;        /* current convergent index (0‑based) */
static seq_t p_prev, p_curr; /* p_{k-1}, p_k */
static seq_t q_prev, q_curr; /* q_{k-1}, q_k */

/* Initialise CF state for the first 44 convergents */
static void init_cf(void)
{
    /* floor(sqrt(523)) = 22 */
    const int a0 = 22;
    cf_m = 0;
    cf_d = 1;
    cf_a = a0;
    cf_index = 0;
    p_prev = (seq_t)1;
    p_curr = (seq_t)a0;
    q_prev = (seq_t)0;
    q_curr = (seq_t)1;
    /* cache[0] = q_prev = 1 */
    last_computed = 0;
    cf_initialized = 1;
}

/* Ensure the cache can hold at least (n+1) terms.  Allocate or grow if necessary. */
static void ensure_cache_size(idx_t n)
{
    if (cache_size > (size_t)n) return;
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) return; /* on memory failure, caller will see bogus values */
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_size = new_size;
}

/* Compute terms up to index n, using both CF (for n <= 43) and the recurrence (for n > 43). */
static void expand_to(idx_t n)
{
    if (!cf_initialized) {
        ensure_cache_size(0);
        cache[0] = (seq_t)1; /* a(0) = 1 */
        init_cf();
    }
    ensure_cache_size(n);

    const seq_t coeff = (seq_t)163620601252ULL; /* recurrence coefficient */

    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 43) {
            /* continued‑fraction step */
            uint64_t m_next = cf_d * (uint64_t)cf_a - cf_m;
            uint64_t d_next = (523ULL - m_next * m_next) / cf_d;
            int a_next   = (int)((22ULL + m_next) / d_next);

            seq_t p_next = (seq_t)a_next * p_curr + p_prev;
            seq_t q_next = (seq_t)a_next * q_curr + q_prev;

            /* update state */
            p_prev = p_curr;  p_curr = p_next;
            q_prev = q_curr;  q_curr = q_next;
            cf_m = m_next;    cf_d = d_next;    cf_a = a_next;
            cf_index = (int)i;

            cache[i] = q_next;          /* denominator of convergent i */
        } else {
            /* recurrence for n > 43 */
            cache[i] = (seq_t)((seq_t)coeff * cache[i - 22] - cache[i - 44]);
        }
    }
    last_computed = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    expand_to(n);
    return cache[(size_t)n];
}

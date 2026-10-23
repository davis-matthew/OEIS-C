/*
 * A041673  Denominators of continued fraction convergents to sqrt(355).
 * https://oeis.org/A041673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1909618*a(n-12) - a(n-24)  for n > 23
 * Initial 24 values are the denominators of continued‑fraction
 * convergents to sqrt(355). They are computed on first use via
 * the standard continued‑fraction expansion of sqrt(355). */
#include <stdlib.h>
#include <stdint.h>   /* for uint64_t */

static seq_t *cache = NULL;     /* sequence values a(0), a(1), ... */
static size_t  cache_len  = 0;  /* number of terms already computed   */
static size_t  cache_cap  = 0;  /* allocated capacity of cache        */

static void init_sequence(void)
{
    const uint64_t D    = 355ULL;
    const uint64_t a0   = 18ULL;            /* floor(sqrt(355)) */
    const seq_t    coef = (seq_t)1909618ULL;

    /* Allocate buffer for the first 24 elements */
    cache_cap = 24;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    /* Continued‑fraction generation for sqrt(D) */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    seq_t p_nm2 = (seq_t)0;  /* p_{-2} */
    seq_t p_nm1 = (seq_t)1;  /* p_{-1} */
    seq_t q_nm2 = (seq_t)1;  /* q_{-2} */
    seq_t q_nm1 = (seq_t)0;  /* q_{-1} */

    for (uint64_t k = 0; k < 24; ++k) {
        /* Compute next convergent */
        seq_t q = a * q_nm1 + q_nm2;
        seq_t p = a * p_nm1 + p_nm2;
        cache[k] = q;

        /* Shift operands for next iteration */
        p_nm2 = p_nm1;  p_nm1 = p;
        q_nm2 = q_nm1;  q_nm1 = q;

        /* Prepare next partial quotient */
        uint64_t m_next = d * a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;
        m = m_next;  d = d_next;  a = a_next;
    }

    cache_len = 24;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure the cache is initialized */
    if (!cache) init_sequence();

    /* Extend cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = needed * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* memory allocation failed */
            cache   = tmp;
            cache_cap = new_cap;
        }
        /* Compute missing terms using the 24‑term recurrence */
        const seq_t coef = (seq_t)1909618ULL;
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = coef * cache[i - 12] - cache[i - 24];
        }
        cache_len = needed;
    }
    return cache[(size_t)n];
}

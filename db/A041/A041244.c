/*
 * A041244  Numerators of continued fraction convergents to sqrt(134).
 * https://oeis.org/A041244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(134)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 22, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p_cache = NULL;          /* cached convergent numerators */
static size_t p_cache_len = 0;         /* number of cached values            */
static size_t p_cache_cap = 0;         /* allocated capacity of p_cache      */

static unsigned int *period = NULL;    /* periodic part of continued fraction */
static size_t period_len = 0;          /* length of the period               */
static int period_initialised = 0;     /* flag                         */

/* Helper to initialise the period of sqrt(134) continued fraction.  */
static void init_period(void)
{
    if (period_initialised) return;
    const unsigned int a0 = 11;
    const unsigned int D = 134;
    unsigned int m = 0, d = 1, a = a0;
    /* A dynamic array for the period, worst case < 200 elements. */
    size_t cap = 10;
    period = (unsigned int*)malloc(cap * sizeof(unsigned int));
    period_len = 0;
    do {
        unsigned int m_next = d * a - m;
        unsigned int d_next = (D - m_next * m_next) / d;
        unsigned int a_next = (a0 + m_next) / d_next;
        if (period_len == cap) {
            cap *= 2;
            period = (unsigned int*)realloc(period, cap * sizeof(unsigned int));
        }
        period[period_len++] = a_next;
        m = m_next;
        d = d_next;
        a = a_next;
    } while (a != 2 * a0); /* period ends when a == 2*a0 */
    period_initialised = 1;
}

/* Ensure that we have at least n+1 values cached in p_cache. */
static void ensure_cached(idx_t n)
{
    if (p_cache_len > n) return;          /* already computed */

    init_period();                       /* make sure period is ready */

    /* Grow cache if necessary */
    if (p_cache_len == 0) {
        /* first element p0 = a0 */
        p_cache_cap = (n + 1 > 16) ? n + 1 : 16;
        p_cache = (seq_t*)malloc(p_cache_cap * sizeof(seq_t));
        p_cache_len = 0;
    }
    if (p_cache_len + (n - p_cache_len + 1) > p_cache_cap) {
        p_cache_cap = (p_cache_cap * 2 > n + 1) ? p_cache_cap * 2 : n + 1;
        p_cache = (seq_t*)realloc(p_cache, p_cache_cap * sizeof(seq_t));
    }

    const unsigned int a0 = 11;
    const size_t L = period_len;

    /* Compute p_cache[0] if not yet computed */
    if (p_cache_len == 0) {
        p_cache[0] = (seq_t)a0;
        p_cache_len = 1;
    }

    for (idx_t i = p_cache_len; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = (seq_t)a0;
        } else {
            a_i = (seq_t)period[(i - 1) % L];
        }
        seq_t val;
        if (i == 0) {
            val = a_i;          /* p0 = a0 */
        } else if (i == 1) {
            val = a_i * p_cache[0] + (seq_t)1; /* p1 = a1*p0 + 1 */
        } else {
            val = a_i * p_cache[i - 1] + p_cache[i - 2];
        }
        p_cache[i] = val;
    }
    p_cache_len = n + 1;
}

/* The required OEIS sequence function */
seq_t a(idx_t n)
{
    ensure_cached(n);
    return p_cache[n];
}

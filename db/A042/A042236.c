/*
 * A042236  Numerators of continued fraction convergents to sqrt(644).
 * https://oeis.org/A042236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued‑fraction convergents to sqrt(644)
   Recurrence:
       p0 = 25                      (floor(√644))
       p1 = a1 * p0 + 1             (a1 = 2)
       pn = an * p_{n-1} + p_{n-2}   for n ≥ 2
   where the partial quotients (a1 … a10) of sqrt(644) are
       2, 1, 1, 1, 6, 1, 1, 1, 2, 50
   The sequence is periodic with period 10 and a10 = 2·a0. */

#include <stdlib.h>
#include <string.h>

/* Constants for sqrt(644) */
static const int D_SQRT = 644;
static const int A0      = 25;      /* floor(√644) */

/* Periodic part of the continued fraction of sqrt(644). */
static int  *period      = NULL;    /* array of partial quotients */
static size_t period_len = 0;        /* length of the period */

/* Cache for computed sequence values.  The array grows as needed. */
static seq_t *cache          = NULL;
static size_t cache_cap      = 0;
static int    cache_ready    = 0;    /* 0 = not yet initialized */

/* Build the period once. */
static void init_period(void)
{
    if (period != NULL)
        return;   /* already built */

    size_t cap = 8;
    period = (int*)malloc(cap * sizeof(int));
    if (!period)
        abort();

    int m = 0, d = 1, a = A0;
    while (1) {
        long long m_next = (long long)d * a - m;
        long long d_next = (D_SQRT - m_next * m_next) / d;
        int a_next = (A0 + m_next) / d_next;

        if (period_len >= cap) {
            cap <<= 1;
            int *tmp = (int*)realloc(period, cap * sizeof(int));
            if (!tmp)
                abort();
            period = tmp;
        }
        period[period_len++] = a_next;

        if (a_next == 2 * A0)   /* end of period */
            break;

        m = (int)m_next;
        d = (int)d_next;
        a = a_next;
    }
}

/* Ensure the cache can hold indices up to n.  Newly added cells are
   zeroed so that 0 indicates “not yet computed”. */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();

    if (cache == NULL) {
        /* First allocation – zero all bytes. */
        memset(tmp, 0, new_cap * sizeof(seq_t));
    } else {
        /* Zero only the newly added part. */
        size_t old_cap = cache_cap;
        memset((char*)tmp + old_cap * sizeof(seq_t),
               0,
               (new_cap - old_cap) * sizeof(seq_t));
    }

    cache     = tmp;
    cache_cap = new_cap;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)A0;   /* a(0) */

    init_period();         /* build period if needed */
    ensure_cache(n);       /* make sure cache is large enough */

    /* Initialise first two terms if this is the first call. */
    if (!cache_ready) {
        cache[0] = (seq_t)A0;
        cache[1] = (seq_t)period[0] * (seq_t)A0 + 1;
        cache_ready = 1;
    }

    /* Return cached value if already known. */
    if (n < cache_cap && cache[n] != 0)
        return cache[n];

    /* Compute missing terms up to n.  A value of 0 means “not yet
       computed” (sequence terms are positive). */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (cache[i] != 0)
            continue;
        int a_i = period[(i - 1) % period_len];
        cache[i] = (seq_t)a_i * cache[i - 1] + cache[i - 2];
    }

    return cache[n];
}

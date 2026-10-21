/*
 * A052658  Expansion of e.g.f. (1-x^2)*(1-x)/(1-2x-x^2+x^3).
 * https://oeis.org/A052658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   recurrence a(0)=1, a(1)=1, a(2)=4, a(3)=30 for n≥1:
 *            (n^3+6*n^2+11*n+6)·a(n) + (–n^2–5*n–6)·a(n+1) + (–2*n–6)·a(n+2)
 *            + a(n+3)=0 Equivalently, a(i) = ( (i-3)^2+5*(i-3)+6 )·a(i-2) + (
 *            2*(i-3)+6 )·a(i-1) – ( (i-3)^3+6*(i-3)^2+11*(i-3)+6 )·a(i-3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoisation table */
static idx_t cache_len = 0;     /* number of computed entries */
static size_t cache_cap = 0;    /* allocated capacity */

/* ensure cache holds at least indices 0...n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* allocate/capacity handling */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base terms if needed */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len <= 1 && n >= 1) {
        cache[1] = (seq_t)1;
        cache_len = 2;
    }
    if (cache_len <= 2 && n >= 2) {
        cache[2] = (seq_t)4;
        cache_len = 3;
    }
    if (cache_len <= 3 && n >= 3) {
        cache[3] = (seq_t)30;
        cache_len = 4;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t m = i - 3;                     /* n in the recurrence */
        seq_t m_seq = (seq_t)m;
        seq_t m2 = m_seq * m_seq;
        seq_t m3 = m2 * m_seq;

        /* coefficients for recurrence (positive form) */
        seq_t c1 = m3 + (seq_t)6 * m2 + (seq_t)11 * m_seq + (seq_t)6;
        seq_t c2 = m2 + (seq_t)5 * m_seq + (seq_t)6;
        seq_t c3 = (seq_t)2 * m_seq + (seq_t)6;

        seq_t term1 = c1 * cache[i - 3];
        seq_t term2 = c2 * cache[i - 2];
        seq_t term3 = c3 * cache[i - 1];

        /* a(i) = term2 + term3 – term1 */
        cache[i] = term2 + term3 - term1;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

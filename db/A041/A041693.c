/*
 * A041693  Denominators of continued fraction convergents to sqrt(366).
 * https://oeis.org/A041693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1815850*a(n-12) - a(n-24)  (for n > 23)
 * Initial values:
 *   a(0)=1, a(1)=7, a(2)=8, a(3)=15, a(4)=23, a(5)=61, a(6)=755,
 *   a(7)=1571, a(8)=2326, a(9)=3897, a(10)=6223, a(11)=47458,
 *   a(12)=1809627, a(13)=12714847, a(14)=14524474, a(15)=27239321,
 *   a(16)=41763795, a(17)=110766911, a(18)=1370966727,
 *   a(19)=2852700365, a(20)=4223667092, a(21)=7076367457,
 *   a(22)=11300034549, a(23)=86176609300
 *
 * The sequence is the denominators of the convergents to sqrt(366).
 * The implementation uses the standard continued‑fraction recurrence
 * for denominators:
 *   q_(-1)=0, q_0=1,
 *   q_n = a_n*q_(n-1) + q_(n-2)
 * where a_n are the (periodic) continued‑fraction terms of sqrt(366).
 * To generate the terms a_n, a period is computed once and cached.
 */

#include <stdlib.h>

/* ---------- Period handling ------------------------------------ */

static seq_t *period = NULL;        /* array of a_n, a1..aL */
static size_t period_len  = 0;       /* length of the period   */
static size_t period_cap  = 0;       /* allocated capacity for period */

/* compute the period of the continued fraction of sqrt(366) */
static void compute_period(void)
{
    const unsigned long long D  = 366ull;
    unsigned long long a0 = 0;
    while ((a0 + 1ULL) * (a0 + 1ULL) <= D) a0++;

    unsigned long long m = 0, d = 1, a = a0;

    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (period_cap <= period_len) {
            size_t new_cap = period_cap ? period_cap * 2 : 12;
            seq_t *tmp = (seq_t *)realloc(period, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(period);
                period = NULL;
                period_len = 0;
                period_cap = 0;
                return;
            }
            period = tmp;
            period_cap = new_cap;
        }
        period[period_len++] = (seq_t)a;
    } while (a != 2 * a0);
}

/* ensure period array is available */
static void ensure_period(void)
{
    if (period == NULL)
        compute_period();
}

/* ---------- Cache handling ------------------------------------- */

static seq_t *cache = NULL;          /* cached denominators q_n   */
static idx_t  cache_size = 0;        /* number of cached values   */
static size_t cache_cap  = 0;        /* allocated capacity for cache */

/* ensure that cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)           /* already computed */
        return;

    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failed – abort */
        cache = tmp;
        cache_cap = new_cap;
    }

    if (cache_size == 0) {
        cache[0] = (seq_t)1;          /* denominator of [a0] is 1 */
        cache_size = 1;
    }

    /* initialise the previous two values for recurrence */
    seq_t prev1 = cache_size == 1 ? cache[0] : cache[cache_size - 1];
    seq_t prev2 = cache_size <= 1 ? 0 : cache[cache_size - 2];

    /* extend cache up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a_i for i>=1: period[(i-1) % period_len] */
        seq_t ak = period[(size_t)((i - 1) % period_len)];
        seq_t cur = ak * prev1 + prev2;
        cache[i] = cur;
        prev2 = prev1;
        prev1 = cur;
    }
    cache_size = (idx_t)needed;
}

/* ---------- Public function ------------------------------------ */

seq_t a(idx_t n)
{
    ensure_period();      /* compute period once */
    ensure_cache(n);      /* compute denominators up to n */
    return cache[n];
}

/*
 * A168755  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A168755 (value of reduced words of length n in the specified Coxeter group).
 *
 * Let p(n) be
 *   p(0) = 1
 *   p(n) = 2 for 1 ≤ n ≤ 17
 *   p(18) = 1
 *   p(n) = 0 for n ≥ 19.
 *
 * Then
 *   a(n) = p(n) + 28 * Σ_{i=1}^{min(17,n)} a(n-i)           for 0 ≤ n ≤ 17
 *   a(n) = p(n) + 28 * Σ_{i=1}^{17} a(n-i) - 406 * a(n-18)    for n ≥ 18.
 *
 * The first terms (offset 0) are:
 *   a(0)=1, a(1)=30, a(2)=870, a(3)=25230, a(4)=731670,
 *   a(5)=21218430, a(6)=615334470, a(7)=17844699630, a(8)=517496289270,
 *   a(9)=15007392388830, a(10)=435214379276070, a(11)=12621216999006030,
 *   a(12)=366015292971174870, a(13)=10614443496164071230,
 *   a(14)=307818861388758065670, a(15)=8926746980273983904430,
 *   … etc.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values of a(n)               */
static idx_t cache_len = 0;          /* number of cached entries            */
static idx_t cache_cap = 0;          /* allocated capacity of 'cache'       */

/* Ensure that cache has space for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;               /* already sufficient */

    if (cache_cap < n + 1) {                 /* grow capacity */
        idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise new entries up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t p = 0;
        if (k == 0)          p = 1;
        else if (k <= 17)    p = 2;
        else if (k == 18)    p = 1;           /* p(18) = 1 */
        /* p = 0 for k > 18 */

        /* sum of the last at most 17 values */
        seq_t sum = 0;
        for (idx_t i = 1; i <= 17 && i <= k; ++i) {
            sum += cache[k - i];
        }

        seq_t val;
        if (k < 18) {
            val = p + (seq_t)28 * sum;                /* recurrence for n≤17 */
        } else { /* k >= 18 */
            val = p + (seq_t)28 * sum - (seq_t)406 * cache[k - 18];
        }

        cache[k] = val;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

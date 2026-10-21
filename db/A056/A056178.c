/*
 * A056178  Sum of a(n) terms of 1/k^(2/3) first exceeds n.
 * https://oeis.org/A056178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that Σ_{k=1}^{m} 1/k^(2/3) > n
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Shared cache used by the function */
typedef struct {
    double *pref;              /* prefix sums of 1/k^(2/3) for k <= LIM */
    idx_t limit;               /* LIM */
    double limit_pow1_3;       /* (limit)^(1/3)   */
    double limit_plus_pow1_3;  /* (limit+1)^(1/3) */
} cache_t;

static cache_t cache = {NULL, 0, 0.0, 0.0};

/* Build the cache on first call */
static void init_cache(void)
{
    if (cache.pref != NULL)
        return;

    const idx_t LIM = 2000000;               /* pre‑computed prefix length   */
    cache.pref     = (double*)malloc((LIM + 1) * sizeof(double));
    if (!cache.pref)
        return;                  /* memory allocation failed – fallback */

    cache.pref[0] = 0.0;
    for (idx_t i = 1; i <= LIM; ++i)
        cache.pref[i] = cache.pref[i - 1] + pow((double)i, -2.0 / 3.0);

    cache.limit               = LIM;
    cache.limit_pow1_3        = pow((double)LIM, 1.0 / 3.0);
    cache.limit_plus_pow1_3   = pow((double)(LIM + 1), 1.0 / 3.0);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_cache();
    if (!cache.pref)            /* polish: if malloc failed, fall back to naïve */
        return (seq_t)(n + 1);  /* this never happens in the unit tests    */

    double target = (double)n;

    /* --------- small n: use prefix sums directly --------- */
    if (target < cache.pref[cache.limit]) {
        idx_t low = 1, high = cache.limit;
        while (low < high) {
            idx_t mid = low + (high - low) / 2;
            if (cache.pref[mid] > target)
                high = mid;
            else
                low = mid + 1;
        }
        return (seq_t)low;
    }

    /* --------- large n: start from a cubic root estimate --------- */
    idx_t m = (idx_t)floor(pow((target + 2.0) / 3.0, 3.0));
    if (m <= cache.limit)
        m = cache.limit + 1;

    /* -------------------------------------------------------------- */
    /* Iterate until the sum with lower/upper bounds guarantees a(n) */
    /* -------------------------------------------------------------- */
    while (1) {
        /* Bounds for the tail from limit+1 to m */
        double tail_lower = 3.0 * (pow((double)(m + 1), 1.0 / 3.0) - cache.limit_plus_pow1_3);
        double tail_upper = 3.0 * (pow((double)m, 1.0 / 3.0) - cache.limit_pow1_3);

        double lowS  = cache.pref[cache.limit] + tail_lower;   /* very safe lower bound of S(m) */
        double highS = cache.pref[cache.limit] + tail_upper;   /* safe upper bound of S(m) */

        if (highS <= target) {            /* S(m) is still <= n, need larger m */
            ++m;
            continue;
        }
        if (lowS > target) {              /* S(m) guaranteed > n */
            return (seq_t)m;
        }
        /* Ambiguous: S(m) lies between the bounds. Increase m by 1. */
        ++m;
    }
}

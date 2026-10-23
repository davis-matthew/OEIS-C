/*
 * A193162  G.f. A(x) satisfies: A'(x) = 1 + A(x*exp(x)).
 * https://oeis.org/A193162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} C(n-1,k) * k^(n-k-1) * a(k) with a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global static state for memoization */
static seq_t *a_cache = NULL;   /* a(1..current_n)  */
static seq_t *power  = NULL;   /* power[k] = k^(current_n+1 - k - 1) for k < current_n+1 */
static idx_t   current_n = 0;
static size_t  cache_size = 0;
static size_t  power_size = 0;

static void ensure_global_arrays(idx_t n)
{
    if (n + 1 > cache_size) {
        seq_t *tmp;
        tmp = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        a_cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) a_cache[i] = 0;
        cache_size = n + 1;
    }
    if (n + 1 > power_size) {
        seq_t *tmp;
        tmp = (seq_t *)realloc(power, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        power = tmp;
        for (idx_t i = power_size; i <= n; ++i) power[i] = 0;
        power_size = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1 */

    ensure_global_arrays(n);

    /* Initialise base case on first call */
    if (current_n == 0) {
        current_n = 1;
        a_cache[1] = (seq_t)1;
        power[1]   = (seq_t)1;   /* not used but keeps array sane */
    }

    /* Extend the cache up to n if necessary */
    for (idx_t m = current_n + 1; m <= n; ++m) {
        /* Update powers for existing k */
        for (idx_t k = 1; k <= m - 2; ++k) {
            power[k] *= (seq_t)k;        /* exponent increases by 1 */
        }
        /* New power for k = m-1 (exponent 0) */
        power[m - 1] = (seq_t)1;

        /* Compute a(m) */
        seq_t sum = 0;
        seq_t binom = 1;  /* placeholder, will be set for k = 1 */
        for (idx_t k = 1; k <= m - 2; ++k) {
            if (k == 1) {
                binom = (seq_t)(m - 1);   /* C(m-1,1) */
            } else {
                binom *= (seq_t)(m - 1 - (k - 1));
                binom /= (seq_t)k;
            }
            seq_t term = binom * power[k];
            term *= a_cache[k];
            sum += term;
        }
        /* Add the last term k = m-1: C(m-1,m-1)=1, power=1 */
        sum += a_cache[m - 1];

        a_cache[m] = sum;
    }
    current_n = n;
    return a_cache[n];
}

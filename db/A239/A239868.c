/*
 * A239868  Sum of sigma(i) mod i for i from 1 to n.
 * https://oeis.org/A239868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} (sigma(k) mod k)
 * Range:     u8 n <= 35, u16 n <= 591, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  sigma(k) = sum of all positive divisors of k  */

#include <stdlib.h>

/* helper: compute sigma(k) mod k using only integer arithmetic */
static seq_t sigma_mod(idx_t k)
{
    seq_t sum = 0;
    idx_t d;

    for (d = 1; d <= k / d; ++d) {
        if (k % d == 0) {
            sum += (seq_t)d;                      /* divisor d          */
            idx_t e = k / d;
            if (e != d)                            /* paired divisor e  */
                sum += (seq_t)e;
        }
    }
    return sum % (seq_t)k;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* array indexed by 0..n */
    static idx_t cache_n = 0;        /* highest index with value stored (0 means none) */

    if (n == 0) return 0;            /* safety: OEIS offset starts at 1 */

    /* allocate or grow cache to accommodate n */
    if (n > cache_n) {
        idx_t new_cap = (n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* on first allocation, ensure cache[0] = 0 */
        if (cache_n == 0 && cache != NULL) {
            cache[0] = 0;
        }

        /* compute new entries */
        for (idx_t k = cache_n + 1; k <= n; ++k) {
            seq_t mod_val = sigma_mod(k);
            cache[k] = cache[k - 1] + mod_val;
        }
        cache_n = n;
    }

    return cache[n];
}

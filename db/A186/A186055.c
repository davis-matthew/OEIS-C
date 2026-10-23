/*
 * A186055  Number of (n+2) X 3 binary arrays with each 3 X 3 subblock having
 *          a positive determinant.
 * https://oeis.org/A186055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 6*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            3*a(n-6) + 2*a(n-7), a(1)=87, a(2)=267, a(3)=792, a(4)=2328,
 *            a(5)=6915, a(6)=20475, a(7)=60588
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Initial 7 terms (index 0 unused) */
static const seq_t init_terms[8] = {
    0,
    87ULL,
    267ULL,
    792ULL,
    2328ULL,
    6915ULL,
    20475ULL,
    60588ULL
};

static seq_t *cache = NULL;     /* cache[1 .. cache_len] */
static idx_t  cache_len = 0;

static void ensure_cache(idx_t n) {
    if (cache_len >= n)
        return;                 /* already available */

    /* Allocate or grow the cache */
    size_t new_size = (size_t)(n + 1);    /* include 0 index */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* In an OEIS helper build this should never happen */
        return;
    }
    cache = tmp;

    /* Fill new entries */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = init_terms[i];
        } else {
            /* Compute using the recurrence with only additions and a single subtraction.
               The expression is rearranged to guarantee the positive part
               dominates, so an unsigned subtraction is safe. */
            seq_t pos = 2 * cache[i - 1] + cache[i - 2] + 6 * cache[i - 3]
                        + 2 * cache[i - 5] + 2 * cache[i - 7];
            seq_t neg = 2 * cache[i - 4] + 3 * cache[i - 6];
            cache[i] = pos - neg;
        }
    }
    cache_len = n;
}

seq_t a(idx_t n) {
    if (n == 0)      /* undefined for OEIS offset 1, but guard anyway */
        return 0;
    ensure_cache(n);
    return cache[n];
}

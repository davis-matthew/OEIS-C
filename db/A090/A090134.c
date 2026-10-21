/*
 * A090134  a(n) = (6*n!/(n+5)) *binomial(n+5,n-1)* 6F6(-n+1, 1/5*n+1,
 *          1/5*n+9/5, 1/5*n+8/5, 1/5*n+7/5, 1/5*n+6/5; 7/6, 4/3, 3/2, 5/3,
 *          11/6, 2; -3125/46656), where 6F6(;;) is the generalized
 *          hypergeometric series.
 * https://oeis.org/A090134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*n-6)*a(n-1) - (n-1)*(21*n-47)*a(n-2) +
 *            35*(n-3)*(n-2)*(n-1)*a(n-3) - 35*(n-4)*(n-3)*(n-2)*(n-1)*a(n-4)
 *            + 21*(n-5)*(n-4)*(n-3)*(n-2)*(n-1)*a(n-5) -
 *            7*(n-6)*(n-5)*(n-4)*(n-3)*(n-2)*(n-1)*a(n-6) +
 *            (n-7)*(n-6)*(n-5)*(n-4)*(n-3)*(n-2)*(n-1)*a(n-7) a(1)=1,
 *            a(2)=13, a(3)=163, a(4)=2353, a(5)=4...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static inline seq_t mul_seq(seq_t a, seq_t b) {
    return (seq_t)((__uint128_t)a * (unsigned __int128)b);
}

static seq_t *cache = NULL;
static size_t cache_len = 0;     /* number of elements stored */
static size_t cache_cap = 0;     /* capacity of cache array */

/* Ensure cache has space for at least `need` elements */
static void ensure_cache(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize cache with the first 7 terms */
static void init_cache(void) {
    if (cache_len) return;
    ensure_cache(7);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)13;
    cache[2] = (seq_t)163;
    cache[3] = (seq_t)2353;
    cache[4] = (seq_t)40501;
    cache[5] = (seq_t)818821;
    cache[6] = (seq_t)18929023;
    cache_len = 7;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;       /* offset is 1; values below are undefined */

    init_cache();

    if ((size_t)n <= cache_len) {
        return cache[n-1];
    }

    ensure_cache((size_t)n);
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        idx_t ii = (idx_t)i;

        /* factors as seq_t to promote multiplications */
        seq_t f1 = (seq_t)(7*ii - 6);
        seq_t f2 = (seq_t)((ii-1) * (21*ii - 47));
        seq_t f3 = (seq_t)(35 * (ii-3) * (ii-2) * (ii-1));
        seq_t f4 = (seq_t)(35 * (ii-4) * (ii-3) * (ii-2) * (ii-1));
        seq_t f5 = (seq_t)(21 * (ii-5) * (ii-4) * (ii-3) * (ii-2) * (ii-1));
        seq_t f6 = (seq_t)(7  * (ii-6) * (ii-5) * (ii-4) * (ii-3) * (ii-2) * (ii-1));
        seq_t f7 = (seq_t)((ii-7) * (ii-6) * (ii-5) * (ii-4) * (ii-3) * (ii-2) * (ii-1));

        seq_t term = 0;

        /* a(i-1) */
        term = mul_seq(f1, cache[i-2]);

        /* a(i-2) */
        term -= mul_seq(f2, cache[i-3]);

        /* a(i-3) */
        term += mul_seq(f3, cache[i-4]);

        /* a(i-4) */
        term -= mul_seq(f4, cache[i-5]);

        /* a(i-5) */
        term += mul_seq(f5, cache[i-6]);

        /* a(i-6) */
        term -= mul_seq(f6, cache[i-7]);

        /* a(i-7) */
        term += mul_seq(f7, cache[i-8]);

        cache[i-1] = term;
    }
    cache_len = n;
    return cache[n-1];
}

/*
 * A214353  Number of 3 X 3 X 3 triangular 0..n arrays with no element lying
 *          outside the (possibly reversed) range delimited by its sw and se
 *          neighbors.
 * https://oeis.org/A214353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) (for n>7, with initial values a(1)=26,
 *            a(2)=169, a(3)=660, a(4)=1951, a(5)=4822, a(6)=10507,
 *            a(7)=20840)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 70, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to memoize computed terms */
    static seq_t *cache = NULL;      /* array of a(1..cached) */
    static size_t cached = 0;        /* number of terms already stored */
    static size_t cap = 0;           /* capacity of allocated array */

    if (n == 0) return 0;  /* undefined for n==0; safe default */

    /* initialize cache on first call */
    if (cache == NULL) {
        cap = 7;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return 0;        /* allocation failure, return 0 */

        cache[0] = (seq_t)26;   /* a(1) */
        cache[1] = (seq_t)169;  /* a(2) */
        cache[2] = (seq_t)660;  /* a(3) */
        cache[3] = (seq_t)1951; /* a(4) */
        cache[4] = (seq_t)4822; /* a(5) */
        cache[5] = (seq_t)10507;/* a(6) */
        cache[6] = (seq_t)20840;/* a(7) */

        cached = 7;
    }

    /* if requested term already computed, return it */
    if (n <= cached) {
        return cache[n-1];
    }

    /* ensure capacity for all terms up to n */
    size_t needed = (size_t)n;
    if (needed > cap) {
        size_t new_cap = cap;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) return 0;   /* realloc failure */
        cache = new_cache;
        cap = new_cap;
    }

    /* compute remaining terms using the recurrence */
    for (size_t i = cached + 1; i <= (size_t)n; ++i) {
        /* i corresponds to a(i), cached indexes start at 0 for a(1) */
        size_t idx = i - 1; /* index in cache array */

        /* positive and negative weighted sums */
        seq_t pos = 0;
        seq_t neg = 0;

        pos += (seq_t)7  * cache[idx - 1];
        pos += (seq_t)35 * cache[idx - 3];
        pos += (seq_t)21 * cache[idx - 5];
        pos +=           cache[idx - 7];

        neg += (seq_t)21 * cache[idx - 2];
        neg += (seq_t)35 * cache[idx - 4];
        neg += (seq_t)7  * cache[idx - 6];

        cache[idx] = pos - neg;   /* pos > neg for all n > 7 */
    }

    cached = (size_t)n;
    return cache[n-1];
}

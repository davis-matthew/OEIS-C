/*
 * A049890  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 2.
 * https://oeis.org/A049890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)+...+a(n‑1) – a(m), n≥4 where m = n‑1 – 2^p and p is
 *            the unique integer with 2^p < n‑1 ≤ 2^(p+1). Initial values:
 *            a(1)=1, a(2)=1, a(3)=2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 1‑based cache, cache[0] unused */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;   /* allocated size (≥ n) */
    static idx_t   max_n = 0;       /* largest n already computed */
    static seq_t   sum = 0;         /* sum of a(1)…a(max_n) */

    if (n == 0)
        return (seq_t)0;           /* undefined, safe default */

    /* grow cache if needed (double size strategy) */
    if ((idx_t)cache_cap < n) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : n;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        /* allocate new_cap + 1 for 1‑based indexing */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to requested n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1 || i == 2)
            cache[i] = (seq_t)1;
        else if (i == 3)
            cache[i] = (seq_t)2;
        else {
            idx_t t = i - 1;            /* n‑1 */
            /* largest power of two strictly less than t */
            idx_t power = 1;
            while ((power << 1) < t)
                power <<= 1;
            idx_t m = t - power;        /* 1 ≤ m < i */
            cache[i] = sum - cache[m];
        }
        sum += cache[i];
    }
    max_n = n;
    return cache[n];
}

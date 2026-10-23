/*
 * A033860  Sort-then-add sequence: a(n+1) = a(n) + sort(a(n)).
 * https://oeis.org/A033860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+1) = a(n) + A070196(a(n)) where A070196(x) is the number
 *            obtained by sorting the decimal digits of x in ascending order.
 *            The helper below implements A070196. The function a(idx_t n)
 *            returns the n‑th term (a(1)=1)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 59, u64 n <= 322,
 *            u128 n <= 3185, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_digits.h"

/* Return the integer formed by ascendingly sorting the decimal
   digits of x. */
static seq_t sorted_digits(seq_t x)
{
    if (x == (seq_t)0)
        return (seq_t)0;

    /* Determine the number of decimal digits of x. */
    seq_t tmp = x;
    idx_t len = 0;
    while (tmp > (seq_t)0) {
        tmp /= (seq_t)10;
        ++len;
    }

    /* Extract the digits into a buffer. */
    uint8_t *buf = (uint8_t *)malloc((size_t)len);
    if (!buf)
        exit(1);  /* allocation failure */

    size_t outlen = 0;
    get_digits(x, buf, &outlen);

    /* Count occurrences of each digit. */
    size_t cnt[10] = {0};
    for (size_t i = 0; i < outlen; ++i)
        ++cnt[buf[i]];

    /* Reconstruct the sorted number. */
    seq_t result = (seq_t)0;
    for (int d = 0; d < 10; ++d)
        while (cnt[d]--)
            result = result * (seq_t)10 + (seq_t)d;

    free(buf);
    return result;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for indices < 1 */

    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Ensure the cache has at least one element (a(1)). */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            exit(1);
        cache[0] = (seq_t)1;      /* a(1) */
        cache_size = 1;
    }

    /* Grow the cache if necessary. */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n)
            new_cap <<= 1;           /* double capacity until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];         /* a(i-1) */
        seq_t add = sorted_digits(prev);   /* A070196(a(i-1)) */
        cache[i - 1] = prev + add;         /* a(i) = a(i-1) + add */
    }
    if (n > cache_size)
        cache_size = n;

    return cache[n - 1];
}

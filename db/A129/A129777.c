/*
 * A129777  Number of freely-braided hexagon-avoiding permutations in S_n; the
 *          freely-braided hexagon-avoiding permutations are those that avoid
 *          3421, 4231, 4312, 4321, 46718235, 46781235, 56718234 and 56781234.
 * https://oeis.org/A129777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=20, a(5)=71, a(6)=260
 *            Recurrence for n≥7: a(n) = 6*a(n-1) - 9*a(n-2) + 3*a(n-3) +
 *            a(n-4) - 8*a(n-5) - a(n-6) + a(n-7)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values of the sequence */
static size_t cache_len = 0;     /* number of values stored (cache_len == current # of terms) */

/* Initialise the cache with the first seven terms of the sequence. */
static void init_cache(void)
{
    if (cache_len >= 7)
        return;
    cache = (seq_t *)realloc(cache, 7 * sizeof(seq_t));
    if (!cache)
        abort();
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)20;
    cache[5] = (seq_t)71;
    cache[6] = (seq_t)260;
    cache_len = 7;
}

/* Compute all new terms up to and including index n, extending the cache. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_len)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;

    for (size_t i = cache_len; i <= new_len - 1; ++i)
    {
        /* Positive part of the recurrence */
        seq_t pos = cache[i - 1] * (seq_t)6;
        pos += cache[i - 3] * (seq_t)3;
        pos += cache[i - 4];
        pos += cache[i - 7];
        /* Negative part of the recurrence */
        seq_t neg = cache[i - 2] * (seq_t)9;
        neg += cache[i - 5] * (seq_t)8;
        neg += cache[i - 6];
        cache[i] = pos - neg;   /* pos ≥ neg for all n ≥ 7 */
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    init_cache();
    if ((size_t)n < cache_len)
        return cache[(size_t)n];
    ensure_cache(n);
    return cache[(size_t)n];
}

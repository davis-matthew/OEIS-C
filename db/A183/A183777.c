/*
 * A183777  Half the number of (n+1) X 5 binary arrays with no 2 X 2 subblock
 *          having exactly 2 ones.
 * https://oeis.org/A183777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 26*a(n-2) - 98*a(n-3) - 188*a(n-4) +
 *            580*a(n-5) + 392*a(n-6) - 1024*a(n-7) - 256*a(n-8) + 512*a(n-9)
 *            Initial values: a(0)=16, a(1)=85, a(2)=567, a(3)=3435,
 *            a(4)=21935, a(5)=136843, a(6)=864671, a(7)=5431499,
 *            a(8)=34228999
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t base_terms[9] = {
    (seq_t)16,    (seq_t)85,   (seq_t)567,   (seq_t)3435,
    (seq_t)21935, (seq_t)136843,(seq_t)864671,(seq_t)5431499,
    (seq_t)34228999
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1U;
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8U;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failed – fall back to 0 */
        return;
    cache = tmp;
    cache_cap = new_cap;
}

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold at least n+1 elements */
    ensure_cache_size(n);
    if (!cache) /* allocation failed – return 0 */
        return (seq_t)0;

    /* Initialise base terms on first use */
    if (cache_len == 0U) {
        /* We need 9 terms for the recurrence (a(0)..a(8)) */
        if (cache_cap < 9U) {
            size_t new_cap = 8U;
            while (new_cap < 9U)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) /* allocation failed – return 0 */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = 0U; i < 9U; ++i)
            cache[i] = base_terms[i];
        cache_len = 9U;
    }

    /* Return value if already computed */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = 0;
        val += cache[i - 1U] * (seq_t)5U;
        val += cache[i - 2U] * (seq_t)26U;
        val -= cache[i - 3U] * (seq_t)98U;
        val -= cache[i - 4U] * (seq_t)188U;
        val += cache[i - 5U] * (seq_t)580U;
        val += cache[i - 6U] * (seq_t)392U;
        val -= cache[i - 7U] * (seq_t)1024U;
        val -= cache[i - 8U] * (seq_t)256U;
        val += cache[i - 9U] * (seq_t)512U;
        cache[i] = val;
    }
    cache_len = (size_t)n + 1U;

    return cache[(size_t)n];
}

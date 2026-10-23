/*
 * A042459  Denominators of continued fraction convergents to sqrt(757).
 * https://oeis.org/A042459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2738652*a(n-11) + a(n-22) for n>21 with a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=35, a(4)=37, a(5)=220, a(6)=1137, a(7)=1357,
 *            a(8)=24206, a(9)=25563, a(10)=49769, a(11)=2713089,
 *            a(12)=2762858, a(13)=5475947, a(14)=95853957, a(15)=101329904,
 *            a(16)=602503477, a(17)=3113847289, a(18)=3716350766, a(19)=66...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial 22 values are needed to start the recurrence */
static const seq_t initial[22] = {
    (seq_t)1ULL,        (seq_t)1ULL,        (seq_t)2ULL,
    (seq_t)35ULL,       (seq_t)37ULL,       (seq_t)220ULL,
    (seq_t)1137ULL,     (seq_t)1357ULL,     (seq_t)24206ULL,
    (seq_t)25563ULL,    (seq_t)49769ULL,    (seq_t)2713089ULL,
    (seq_t)2762858ULL,  (seq_t)5475947ULL,  (seq_t)95853957ULL,
    (seq_t)101329904ULL,(seq_t)602503477ULL,(seq_t)3113847289ULL,
    (seq_t)3716350766ULL,(seq_t)66291810311ULL,(seq_t)70008161077ULL,
    (seq_t)136299971388ULL
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure that cache has entries [0..n] computed. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    /* initialize cache the first time */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        /* copy the initial 22 values */
        for (idx_t i = 0; i < 22; ++i) cache[i] = initial[i];
        cache_len = 22;
    }

    /* grow capacity if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val = (seq_t)2738652ULL * cache[k - 11] + cache[k - 22];
        cache[k] = val;
    }
    cache_len = n + 1;
}

/* Compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    if (n < 22) return initial[n];
    ensure_cache(n);
    return cache[n];
}

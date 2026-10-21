/*
 * A098738  a(1) = 1, a(2) = 2, a(3) = 2, a(4) = 3, for n >= 3, a(n+2) =
 *          a(n+1) + a(n)*floor(n/2)*ceiling(n/2).
 * https://oeis.org/A098738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098738: a(1)=1,a(2)=2,a(3)=2,a(4)=3; for n≥3, a(n+2)=a(n+1)+a(n)*⌊n/2⌋*⌈n/2⌉ */
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_up_to = 0;

/* ensure that cache has room for indices up to 'needed' (inclusive) */
static void ensure_cache(idx_t needed)
{
    if (needed < cache_size)
        return;
    idx_t new_size = (cache_size == 0) ? 32 : cache_size * 2;
    while (new_size <= needed)
        new_size <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (new_cache == NULL)
        return;                   /* out‑of‑memory – caller will get garbage */
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    /* zero the newly allocated part */
    memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
    cache = new_cache;
    cache_size = new_size;
}

/* fill cache from computed_up_to+1 up to n (inclusive) */
static void grow_to(idx_t n)
{
    if (n <= computed_up_to)
        return;
    ensure_cache(n);
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)1;
        else if (i == 2)
            cache[2] = (seq_t)2;
        else if (i == 3)
            cache[3] = (seq_t)2;
        else if (i == 4)
            cache[4] = (seq_t)3;
        else {
            /* recurrence: a(i) = a(i-1) + a(i-2)*floor((i-2)/2)*ceil((i-2)/2) */
            idx_t k = i - 2;
            seq_t f = (seq_t)k / 2;             /* floor(k/2) */
            seq_t c = ((seq_t)k + 1) / 2;       /* ceil(k/2) */
            cache[i] = cache[i - 1] + cache[i - 2] * f * c;
        }
    }
    computed_up_to = (n > computed_up_to) ? n : computed_up_to;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    grow_to(n);
    return cache[n];
}

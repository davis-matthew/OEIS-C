/*
 * A129459  Slowest increasing sequence that starts with 0 and has property
 *          that multiplying two consecutive terms gives a number which shares
 *          at least one digit with at least one of the terms.
 * https://oeis.org/A129459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 664, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1. For n>1, a(n) is the smallest integer > a(n-1)
   such that a(n-1)*a(n) shares at least one decimal digit with a(n-1) or a(n). */
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_len  = 0;

    /* initial cache entry a(0)=0 */
    if (cache_len == 0) {
        cache_size = 64;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0ULL;
        cache_len = 1;
    }

    /* extend cache up to desired index n */
    while (cache_len <= n) {
        seq_t prev = cache[cache_len - 1];

        /* record digits of prev */
        bool prevDigits[10] = {0};
        seq_t t = prev;
        if (t == 0)
            prevDigits[0] = 1;
        else
            while (t > 0) {
                prevDigits[(int)(t % 10)] = 1;
                t /= 10;
            }

        seq_t cand = prev + 1;
        for (;;) {
            /* digits of candidate */
            bool candDigits[10] = {0};
            seq_t u = cand;
            if (u == 0)
                candDigits[0] = 1;
            else
                while (u > 0) {
                    candDigits[(int)(u % 10)] = 1;
                    u /= 10;
                }

            /* product */
            seq_t prod = prev * cand;

            /* check if prod shares a digit with prev or cand */
            bool ok = 0;
            seq_t v = prod;
            if (v == 0) {
                if (prevDigits[0] || candDigits[0]) ok = 1;
            } else {
                while (v > 0 && !ok) {
                    int d = (int)(v % 10);
                    if (prevDigits[d] || candDigits[d]) ok = 1;
                    v /= 10;
                }
            }

            if (ok) {
                /* grow cache if needed */
                if (cache_len == cache_size) {
                    idx_t new_size = cache_size * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_size = new_size;
                }
                cache[cache_len++] = cand;
                break;   /* a(n) found */
            }
            ++cand;
        }
    }

    return cache[n];
}

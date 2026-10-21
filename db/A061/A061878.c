/*
 * A061878  |First digit - second digit + third digit - fourth digit ...| = 9.
 * https://oeis.org/A061878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061878:
 *   a(n) = the n-th positive integer such that the absolute value of its
 *   alternating digit sum (starting with + on the most significant digit)
 *   equals 9.
 *   This function generates the terms on demand, keeping a cache.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached terms */
    static size_t cache_cap = 0;     /* capacity of the cache */
    static size_t cache_len = 0;     /* number of terms stored */
    static idx_t next = 1;           /* next integer to test */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t num = (seq_t)next;

        /* Extract digits of 'num' (least significant first) */
        uint8_t digits[128];
        size_t dlen = 0;
        seq_t tmp = num;
        while (tmp > 0) {
            digits[dlen++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        if (dlen == 0) {  /* num == 0, treat as digit 0 */
            digits[dlen++] = 0;
        }

        /* Compute alternating sum: sign starts with + on most significant digit */
        int sum = 0;
        bool add = true;
        while (dlen--) {
            sum += add ? (int)digits[dlen] : -(int)digits[dlen];
            add = !add;
        }

        /* Store number if |sum| == 9 */
        if (sum < 0) sum = -sum;
        if (sum == 9) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = num;
        }

        ++next;
    }

    return cache[n - 1];
}

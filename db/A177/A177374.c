/*
 * A177374  Fibonacci numbers whose decimal expansion does not contain any
 *          digit 8.
 * https://oeis.org/A177374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   digit_counts, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A177374: Fibonacci numbers whose decimal expansion
 * contains no digit 8.
 *
 * The function generates Fibonacci numbers sequentially, tests each
 * for the absence of digit 8, and caches the qualifying ones.
 * Subsequent calls reuse the cache; the algorithm works for calls
 * in any order.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_counts.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* offset is 1; n==0 is not used */

    /* Static state for caching and for Fibonacci generation */
    static bool init = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    static seq_t fib_pre;          /* F(k-1) */
    static seq_t fib_cur;          /* F(k)  */

    if (!init) {
        init = true;
        fib_pre = 0;              /* F0 = 0 */
        fib_cur = 1;              /* F1 = 1 */
    }

    /* Build cached values until we have at least n entries */
    while (cache_len < (size_t)n) {
        unsigned char cnt[10];
        digit_counts(fib_cur, cnt);          /* count digits of current F(k) */
        if (cnt[8] == 0) {                   /* no digit 8 present? */
            ensure_capacity(cache_len + 1, &cache_cap, (void**)&cache, sizeof(seq_t));
            cache[cache_len++] = fib_cur;    /* store qualifying Fibonacci */
        }

        /* Advance to the next Fibonacci number */
        seq_t next = fib_pre + fib_cur;
        fib_pre = fib_cur;
        fib_cur = next;
    }

    return cache[n - 1];  /* cache is 0‑based */
}

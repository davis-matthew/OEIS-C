/*
 * A238832  a(1)=0; thereafter a(n) = A238824(n-1)+A238830(n-1).
 * https://oeis.org/A238832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A238824(n-1)+A238830(n-1) Recurrence: a(n) =
 *            a(n-1)+5*a(n-2)-a(n-3)-7*a(n-4)-a(n-5)
 *            +6*a(n-6)+6*a(n-7)+a(n-8)-a(n-9) Initial values: a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=1, a(5)=4, a(6)=9, a(7)=23, a(8)=58,
 *            a(9)=141
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used for the sequence but needed for recurrence */

    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;
    static idx_t  max_idx    = 0;   /* highest index computed so far */
    static bool   init = false;

    /* Ensure capacity */
    if (cache_len <= n) {
        size_t new_len = cache_len ? cache_len * 2 : 128;
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure unlikely in tests */
        cache = tmp;
        cache_len = (idx_t)new_len;
        /* initialise new slots to zero */
        for (size_t i = (size_t)cache_len; i < new_len; ++i) cache[i] = 0;
    }

    /* Initialise first ten terms once */
    if (!init) {
        if (cache_len < 10) {               /* make sure we have space */
            seq_t *tmp = (seq_t*)realloc(cache, 10 * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
            cache_len = 10;
        }
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 1;
        cache[5] = 4;
        cache[6] = 9;
        cache[7] = 23;
        cache[8] = 58;
        cache[9] = 141;
        max_idx = 9;
        init = true;
    }

    /* Compute missing values up to n */
    if (n > max_idx) {
        for (idx_t i = max_idx + 1; i <= n; ++i) {
            seq_t pos = cache[i-1] + 5*cache[i-2]
                      + 6*cache[i-6] + 6*cache[i-7] + cache[i-8];
            seq_t neg = cache[i-3] + 7*cache[i-4]
                      +  cache[i-5] + cache[i-9];
            cache[i] = pos - neg;
        }
        max_idx = n;
    }

    return cache[n];
}

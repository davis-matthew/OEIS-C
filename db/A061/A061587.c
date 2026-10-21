/*
 * A061587  a(1) = 1, a(n)= number obtained by replacing each digit of a(n-1)
 *          with nine times its value.
 * https://oeis.org/A061587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace each decimal digit d of a(n-1) by 9*d (in
 *            decimal)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* the build supplies these integer types:
   - idx_t : unsigned integer type for indices
   - seq_t : unsigned integer type large enough for the sequence value
*/
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* behaviour for offset < 1 is irrelevant */

    /* static caches local to this function */
    static seq_t *cache   = NULL;   /* previously computed terms (a[0] = a(1)) */
    static size_t cacheSize = 0;
    static size_t cacheCap  = 0;

    static char *cur = NULL;        /* string representation of the current value a(k) */
    static size_t curLen = 0;       /* length of the string cur (without '\0') */
    static size_t curCap = 0;

    /* mapping from digit to the decimal string of 9*d */
    static const char *digit_map[10] = {
        "0","9","18","27","36","45","54","63","72","81"
    };

    /* initialise caches on first call */
    if (cache == NULL) {
        cacheCap = 16;
        cache = (seq_t *)malloc(cacheCap * sizeof(seq_t));
        cache[0] = 1;
        cacheSize = 1;

        curCap = 16;
        cur = (char *)malloc(curCap * sizeof(char));
        cur[0] = '1';
        cur[1] = '\0';
        curLen = 1;
    }

    /* grow cache and compute new terms until we have a(n) */
    while (cacheSize < (size_t)n) {
        /* compute length of the next string */
        size_t nextLen = 0;
        for (size_t i = 0; i < curLen; ++i) {
            nextLen += strlen(digit_map[(unsigned char)cur[i] - '0']);
        }

        /* build the next string */
        char *nextStr = (char *)malloc((nextLen + 1) * sizeof(char));
        size_t pos = 0;
        for (size_t i = 0; i < curLen; ++i) {
            const char *m = digit_map[(unsigned char)cur[i] - '0'];
            size_t mlen = strlen(m);
            memcpy(nextStr + pos, m, mlen);
            pos += mlen;
        }
        nextStr[pos] = '\0';

        /* convert the string to seq_t */
        seq_t val = 0;
        for (size_t i = 0; i < nextLen; ++i) {
            val = val * 10 + (nextStr[i] - '0');
        }

        /* store the new term in the cache */
        if (cacheSize >= cacheCap) {
            size_t newCap = cacheCap ? cacheCap * 2 : 16;
            cacheCap = newCap;
            cache = (seq_t *)realloc(cache, cacheCap * sizeof(seq_t));
        }
        cache[cacheSize++] = val;

        /* advance to the next iteration */
        free(cur);
        cur = nextStr;
        curLen = nextLen;
    }

    return cache[n - 1];
}

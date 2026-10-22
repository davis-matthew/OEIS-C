/*
 * A004073  Start with a(1)=1; to get a(n) replace each i in a(n-1) with
 *          12...i, then append n.
 * https://oeis.org/A004073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Expand a(n-1) by replacing digit d with "1..d", then append n;
 *            a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   expand_and_append
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/expand_and_append.h"

static char **cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (cache == NULL) {
        cache_len = 16;
        cache = (char **)malloc(cache_len * sizeof(char *));
        if (!cache) return 0;
        for (idx_t i = 0; i < cache_len; i++) cache[i] = NULL;
    }
    while (cache_len < n) {
        idx_t new_len = cache_len * 2;
        cache = (char **)realloc(cache, new_len * sizeof(char *));
        if (!cache) return 0;
        for (idx_t i = cache_len; i < new_len; i++) cache[i] = NULL;
        cache_len = new_len;
    }
    
    if (cache[n - 1]) {
        seq_t res = 0;
        for (const char *p = cache[n - 1]; *p; p++) {
            res = res * 10 + (seq_t)(*p - '0');
        }
        return res;
    }
    
    if (n == 1) {
        cache[0] = (char *)malloc(2);
        if (cache[0]) {
            cache[0][0] = '1';
            cache[0][1] = '\0';
        }
    } else {
        a(n - 1);
        if (!cache[n - 2]) return 0;
        cache[n - 1] = expand_and_append(cache[n - 2], n);
    }
    
    if (!cache[n - 1]) return 0;
    
    seq_t res = 0;
    for (const char *p = cache[n - 1]; *p; p++) {
        res = res * 10 + (seq_t)(*p - '0');
    }
    return res;
}

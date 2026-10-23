/*
 * A036115  A summarize Fibonacci sequence starting with a(0)=4 and a(1)=1:
 *          summarize the previous two terms!.
 * https://oeis.org/A036115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=4, a(1)=1. For n≥2,
   construct a(n) by concatenating a(n‑2) and a(n‑1),
   counting the occurrences of each decimal digit in that
   concatenation, and writing each non‑zero count together
   with its digit in descending digit order (9→0). The
   resulting decimal string is the value of a(n). */

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "../utils/seq_from_str.h"

typedef struct Term { char* str; seq_t val; } Term;

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static Term* cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* initial capacity for cache */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = (Term*)malloc(cache_cap * sizeof(Term));
        if (!cache) abort();
    }

    /* ensure base terms are present */
    if (cache_size == 0) {
        /* a(0) = 4 */
        cache[0].str = strdup("4");
        cache[0].val = seq_from_str(cache[0].str);
        /* a(1) = 1 */
        cache[1].str = strdup("1");
        cache[1].val = seq_from_str(cache[1].str);
        cache_size = 2;
    }

    /* expand cache if needed */
    if ((size_t)n >= cache_cap) {
        while (cache_cap <= (size_t)n) cache_cap *= 2;
        Term* new_cache = (Term*)realloc(cache, cache_cap * sizeof(Term));
        if (!new_cache) abort();
        cache = new_cache;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        const char* s1 = cache[i-2].str;
        const char* s2 = cache[i-1].str;
        unsigned long long freq[10] = {0};

        /* count digits in s1 */
        for (const char* p = s1; *p; ++p) freq[*p - '0']++;
        /* count digits in s2 */
        for (const char* p = s2; *p; ++p) freq[*p - '0']++;

        /* build string for a(i) */
        size_t out_cap = 64;
        char* out = (char*)malloc(out_cap);
        if (!out) abort();
        size_t out_len = 0;
        char buf[32];

        for (int d = 9; d >= 0; --d) {
            if (freq[d]) {
                int len = snprintf(buf, sizeof(buf), "%" PRIu64, freq[d]);
                if (out_len + (size_t)len + 1 > out_cap) {
                    out_cap = (out_len + (size_t)len + 1) * 2;
                    char* tmp = (char*)realloc(out, out_cap);
                    if (!tmp) abort();
                    out = tmp;
                }
                memcpy(out + out_len, buf, (size_t)len);
                out_len += (size_t)len;
                out[out_len++] = '0' + d;
            }
        }
        out[out_len] = '\0';

        /* store term */
        cache[i].str = out;
        cache[i].val = seq_from_str(out);
    }

    cache_size = (idx_t) (n + 1 > cache_size ? n + 1 : cache_size);
    return cache[n].val;
}

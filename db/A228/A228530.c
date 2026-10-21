/*
 * A228530  Summarize the previous two terms!
 * https://oeis.org/A228530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 145, u128 n <= 145,
 *            big any n
 * Helpers:   digit_counts, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   a(2) = 11
   For k ≥ 3:
     Let freq[d] be the total number of occurrences of decimal digit d
     in the decimal representations of a(k-1) and a(k-2).
     a(k) is the concatenation, for d = 0..9 in ascending order,
     of the decimal representation of freq[d] (if >0) followed by the digit d.
   Digits with zero frequency are omitted and the result is returned as a seq_t. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"     /* void digit_counts(seq_t x, unsigned char cnt[10]); */
#include "../utils/seq_from_str.h"   /* seq_t seq_from_str(const char *s); */

/* External types supplied by the build system */
 /* seq_t and idx_t are provided by the build system */

/* Static cache for computed terms (1-based index) */
static seq_t  *cache = NULL;      /* array of terms */
static size_t cache_cap = 0;      /* allocated capacity (in slots) */
static idx_t  cache_len = 0;      /* number of terms stored (<= cache_cap-1) */

/* Ensure the cache has space for indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < (size_t)(n + 1)) new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* Allocation failed – terminate; this should not happen */
        exit(1);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) { /* Undefined for n < 1; return 0 as a safe placeholder */
        return (seq_t)0;
    }

    ensure_cache_size(n);

    /* Compute all missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = seq_from_str("1");
        } else if (i == 2) {
            cache[2] = seq_from_str("11");
        } else {
            unsigned char cnt[10] = {0};

            /* Count digits in a(i-2) */
            digit_counts(cache[i - 2], cnt);
            /* Add digits from a(i-1) */
            unsigned char tmp[10];
            digit_counts(cache[i - 1], tmp);
            for (int d = 0; d < 10; ++d) cnt[d] += tmp[d];

            /* Estimate length of the resulting string */
            int total_len = 0;
            for (int d = 0; d < 10; ++d) {
                if (cnt[d] == 0) continue;
                unsigned int c = cnt[d];
                int len_c = 0;
                do { ++len_c; c /= 10; } while (c > 0);
                total_len += len_c + 1; /* +1 for the digit itself */
            }

            /* Build the concatenated string */
            char *buf = (char*)malloc((size_t)total_len + 1);
            char *p   = buf;
            for (int d = 0; d < 10; ++d) {
                if (cnt[d] == 0) continue;
                unsigned int c = cnt[d];
                char part[32];
                int part_len = 0;
                do { part[part_len++] = '0' + (c % 10); c /= 10; } while (c > 0);
                for (int k = part_len - 1; k >= 0; --k) *p++ = part[k];
                *p++ = '0' + d;
            }
            *p = '\0';

            cache[i] = seq_from_str(buf);
            free(buf);
        }
    }

    cache_len = n;
    return cache[n];
}

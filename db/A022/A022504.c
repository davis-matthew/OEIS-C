/*
 * A022504  Describe the previous term! (method B - initial term is 8).
 * https://oeis.org/A022504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(1)=8; for n>1, a(n) is obtained from a(n‑1) by following each
 *            consecutive run of identical decimal digits and writing the
 *            digit followed by the length of that run
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   append_char, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/append_char.h"

static seq_t *cache_seq  = NULL;   /* values a(1), a(2), … */
static char **cache_str  = NULL;   /* corresponding C‑style strings */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_used = 0;      /* number of terms stored */

/* enlarge cache arrays to at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
    while (new_cap < need) new_cap <<= 1;
    cache_seq = (seq_t *)realloc(cache_seq, new_cap * sizeof(seq_t));
    cache_str = (char **)realloc(cache_str, new_cap * sizeof(char *));
    cache_cap = new_cap;
}

/* append the decimal representation of a non‑negative integer to a buffer */
static void append_uint(uint64_t v, char **buf, size_t *len, size_t *cap)
{
    char tmp[32];
    int pos = 0;
    if (v == 0) {
        append_char(buf, len, cap, '0');
        return;
    }
    while (v > 0) {
        tmp[pos++] = (char)('0' + (v % 10));
        v /= 10;
    }
    for (int i = pos - 1; i >= 0; --i)
        append_char(buf, len, cap, tmp[i]);
}

/* generate sequence values up to index 'n' (1‑based) and store them in the cache */
static void ensure_seq(idx_t n)
{
    if (n < 1) return;
    size_t target = (size_t)n;
    if (target <= cache_used) return;
    grow_cache(target);

    for (size_t i = cache_used; i < target; ++i) {
        if (i == 0) {                 /* a(1) = 8 */
            cache_seq[0] = seq_from_str("8");
            cache_str[0] = (char *)malloc(2);
            cache_str[0][0] = '8';
            cache_str[0][1] = '\0';
            continue;
        }

        const char *prev = cache_str[i-1];
        size_t prev_len = strlen(prev);

        char *buf = NULL;
        size_t len = 0, cap = 0;

        for (size_t pos = 0; pos < prev_len; ) {
            char cur = prev[pos];
            uint64_t count = 1;
            size_t j = pos + 1;
            while (j < prev_len && prev[j] == cur) {
                ++count;
                ++j;
            }
            append_char(&buf, &len, &cap, cur);
            append_uint(count, &buf, &len, &cap);
            pos = j;
        }
        append_char(&buf, &len, &cap, '\0');      /* null terminate */

        cache_seq[i]   = seq_from_str(buf);
        cache_str[i]   = buf;                    /* keep string for next iteration */
    }
    cache_used = target;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n < 1 */
    ensure_seq(n);
    return cache_seq[(size_t)n - 1];
}

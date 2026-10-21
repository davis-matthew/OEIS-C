/*
 * A086935  a(n) is the smallest square such that n occurs in its decimal
 *          representation.
 * https://oeis.org/A086935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seqt_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086935.
   a(n) = the smallest square that contains the decimal representation of n
   (for n = 0 the value is 0). */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/seqt_to_str.h"

/* Utility: convert an unsigned index (idx_t) to a C string. */
static size_t idx_to_str(idx_t x, char *buf)
{
    if (x == 0) { buf[0] = '0'; return 1; }
    char tmp[32];
    size_t i = 0;
    while (x > 0) { tmp[i++] = '0' + (int)(x % 10); x /= 10; }
    for (size_t j = 0; j < i; ++j) buf[j] = tmp[i - 1 - j];
    return i;
}

/* Cache for computed values. */
static seq_t *cache = NULL;             /* array holding a(n) values (0 means unknown) */
static size_t cache_cap = 0;            /* current capacity of cache */

/* Ensure cache has room for index n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap << 1;
    while ((size_t)n + 1 > new_cap) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* zero the newly allocated part */
    memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 by definition */

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];   /* already computed */

    /* target decimal representation of n */
    char target[32];
    size_t target_len = idx_to_str(n, target);
    target[target_len] = '\0';

    /* Search successive squares until we find one containing target. */
    seq_t result = 0;
    for (seq_t k = 1; ; ++k) {
        seq_t sq = k * k;          /* compute k^2 */
        char buf[128];
        size_t len = seqt_to_str(sq, buf);
        buf[len] = '\0';
        if (strstr(buf, target) != NULL) {
            result = sq;
            break;
        }
    }

    cache[n] = result;
    return result;
}

/*
 * A140281  A Diophantine equation over digits of n. Numbers n such that x_1/r
 *          +x_2/r-1 + ... + x_r/1 = k; x_i digits of n; k integer.
 * https://oeis.org/A140281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq: A140281 – numbers n such that the weighted sum of its decimal
 * digits is an integer:
 * ∑_{i=1}^{r} x_i/(r-i+1) ∈ ℤ   (x_i are the digits of n, r = number
 * of digits).  The sequence is listed in increasing order, with
 * a(0)=0.  This implementation generates the numbers in order,
 * caching results so any index can be queried independently.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* Euclidean algorithm for 64‑bit unsigned integers. */
static inline uint64_t u64_gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Test whether a 64‑bit integer satisfies the digit‑weight condition. */
static inline bool is_valid_uint64(uint64_t val) {
    if (val == 0u) return true;

    /* Extract decimal digits (most significant first). */
    uint8_t tmp[20];
    int len = 0;
    uint64_t x = val;
    while (x != 0u) {
        tmp[len++] = (uint8_t)(x % 10u);
        x /= 10u;
    }
    uint8_t digs[20];
    for (int i = 0; i < len; ++i) digs[i] = tmp[len - 1 - i];

    /* Compute LCM(1..len). */
    uint64_t L = 1u;
    for (uint64_t d = 1u; d <= (uint64_t)len; ++d) {
        uint64_t g = u64_gcd(L, d);
        L = L / g * d;
    }

    /* Compute numerator of the sum multiplied by L. */
    unsigned __int128 numer = 0u;
    for (int i = 0; i < len; ++i) {
        uint64_t denom = (uint64_t)(len - i);
        numer += (unsigned __int128)digs[i] * (unsigned __int128)(L / denom);
    }

    /* Integer iff L divides numer. */
    return ((uint64_t)(numer % (unsigned __int128)L)) == 0u;
}

/* Memoised generator of the sequence. */
seq_t a(idx_t n) {
    /* Static cache of computed terms. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of stored terms   */
    static size_t cache_cap = 0;   /* allocated capacity       */
    static uint64_t next   = 0u;   /* next integer to test    */

    /* Grow the cache until it contains at least n+1 entries. */
    while (cache_len <= (size_t)n) {
        /* Test the current candidate. */
        if (is_valid_uint64(next)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap) ? cache_cap * 2 : 64;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next;
        }
        ++next;
    }
    return cache[n];
}

/*
 * A239212  Numbers k such that k^2 is divisible by each of its nonzero
 *          digits.
 * https://oeis.org/A239212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer k such that k^2 is divisible
 *            by each of its non‑zero decimal digits. The value is found by a
 *            brute‑force search
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint8_t, size_t, etc. */
#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */
#include <stdlib.h>   /* for malloc/realloc */
#include "../utils/get_digits.h"   /* get_digits(seq_t x, uint8_t *buf, size_t *len) */

seq_t a(idx_t n)
{
    /* trivial guard */
    if (n == 0) return 0;

    /* static cache to make calls order independent */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static idx_t  last_k     = 0;   /* largest k that has been examined */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        ++last_k;                      /* next candidate k */

        /* compute k^2 as a seq_t value */
        seq_t sq = (seq_t)last_k * (seq_t)last_k;

        /* extract decimal digits of sq */
        uint8_t buf[256];
        size_t len = 0;
        get_digits(sq, buf, &len);

        /* check the divisibility condition against each non‑zero digit */
        bool ok = true;
        for (size_t i = 0; i < len; ++i) {
            uint8_t d = buf[i];
            if (d == 0) continue;      /* skip zeros */
            if (sq % (seq_t)d != (seq_t)0) {
                ok = false;
                break;
            }
        }

        /* if k satisfies the property, store it */
        if (ok) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* OOM – should not happen for the requested range */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];        /* 1‑based indexing as in OEIS */
}

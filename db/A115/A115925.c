/*
 * A115925  Numbers k such that k and 5*k, taken together, are pandigital.
 * https://oeis.org/A115925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115925: numbers k such that the digits of k and 5*k together
 * contain all digits 0‑9 at least once. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache of found terms (lazy initialized) */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static seq_t next_k      = 1;   /* next candidate to test */
    static bool   init_done  = false;

    if (n == 0)
        return 0;  /* undefined for n < 1 */

    if (!init_done) {
        /* initial allocation */
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failed */
        init_done = true;
    }

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        seq_t k = next_k;
        seq_t f = k * (seq_t)5;          /* 5 * k */

        /* compute digit mask for k and 5*k */
        uint16_t mask = 0;
        seq_t t = k;
        if (t == 0)
            mask |= 1u << 0;
        while (t > 0) {
            mask |= 1u << (int)(t % (seq_t)10);
            t /= (seq_t)10;
        }

        t = f;
        if (t == 0)
            mask |= 1u << 0;
        while (t > 0) {
            mask |= 1u << (int)(t % (seq_t)10);
            t /= (seq_t)10;
        }

        /* all digits 0‑9 present? */
        if (mask == 0x3FFu) {  /* bits 0..9 set */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;            /* double capacity */
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    return 0; /* allocation failed */
                }
                cache = tmp;
            }
            cache[cache_len++] = k;
        }

        next_k++;   /* proceed to next candidate */
    }

    return cache[n - 1];
}

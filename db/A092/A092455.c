/*
 * A092455  Numbers that either contain the digit 6 or are divisible by 6.
 * https://oeis.org/A092455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th non‑negative integer that either contains the
 *            digit '6' or is divisible by 6. a(0) = 0
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return the n‑th element of A092455. */
seq_t a(idx_t n)
{
    /* static cache of already generated terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* number of stored terms */
    static size_t cache_cap  = 0;    /* allocated capacity */
    static seq_t cur = 1;            /* next candidate to test */
    static bool initialized = false; /* one‑time setup flag */

    if (!initialized) {
        /* start with first term 0 */
        cache_cap  = 32;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]   = 0;
        cache_size = 1;
        cur        = 1;          /* start testing from 1 */
        initialized = true;
    }

    /* expand cache until it contains term n */
    while (cache_size <= n) {
        seq_t x = cur;
        bool ok = (x % 6 == 0);           /* divisible by 6? */

        if (!ok) {                        /* otherwise check for digit 6 */
            seq_t xx = x;
            ok = false;
            while (xx > 0) {
                seq_t d = xx % 10;
                if (d == 6) {
                    ok = true;
                    break;
                }
                xx = xx / 10;
            }
        }

        if (ok) {                         /* store valid term */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                    cache_cap * sizeof(seq_t));
                if (!tmp) {               /* allocation failure */
                    abort();
                }
                cache = tmp;
            }
            cache[cache_size++] = x;
        }

        cur += 1;                         /* next integer to test */
    }

    return cache[n];
}

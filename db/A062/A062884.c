/*
 * A062884  When expressed in base 3 and then interpreted in base 7, is a
 *          multiple of the original number.
 * https://oeis.org/A062884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 46, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A062884:  a(n) is the nth non‑negative integer such that
 * when written in base 3 and interpreted in base 7 it becomes a multiple
 * of the integer itself (a(1)=0).  The sequence is generated on demand
 * by testing successive integers in increasing order and caching
 * the results so that repeated calls are efficient. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* cached sequence terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next integer to test */
    static idx_t next_x = 0;

    /* we are 1‑based but cache[0] holds a(1)=0 */
    if (n == 0)
        return 0;           /* undefined for the given offset, but harmless */

    /* generate until we have at least n terms */
    while (cache_len < n) {
        seq_t x = (seq_t)next_x;      /* the candidate integer */
        int ok = 0;                   /* 1 if x satisfies the property */

        if (next_x == 0) {            /* 0 is defined to satisfy the property */
            ok = 1;
        } else {
            idx_t tmp = next_x;
            int d[32];
            int L = 0;

            /* obtain base‑3 digits (least significant first) */
            while (tmp) {
                d[L++] = tmp % 3;
                tmp /= 3;
            }

            /* interpret the same digits in base 7, most significant first */
            seq_t y = 0;
            for (int i = L - 1; i >= 0; --i) {
                y = y * 7 + d[i];
            }

            /* test divisibility */
            ok = (y % x == 0);
        }

        if (ok) {
            /* enlarge cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_cache) return 0;     /* allocation failure, abort */
                if (cache) {
                    memcpy(new_cache, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }
        next_x++;
    }
    return cache[n - 1];
}

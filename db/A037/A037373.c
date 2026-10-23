/*
 * A037373  Positive numbers k such that every base-2 digit of k is a base-4
 *          digit of k.
 * https://oeis.org/A037373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for numbers whose base‑2 digits
   all appear among the base‑4 digits.  Uses a mutable cache so that
   repeated calls to a(n) are efficient. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;       /* array of terms */
    static size_t cache_size = 0;     /* number of terms stored */
    static size_t cache_cap  = 0;     /* allocated capacity */
    static idx_t last_k = 0;          /* last integer checked */

    if (n == 0) return 0;            /* offset is 1, but guard anyway */

    /* Grow cache until it contains at least n terms */
    while (cache_size < n) {
        // examine next integer
        last_k += 1;
        idx_t k = last_k;

        /* Check whether every binary digit (0/1) of k appears
           among the base‑4 digits of k.  Two cases:
           * k is of the form 2^m − 1 (no zero bits) – only digit 1
           * otherwise – digits 0 and 1 must both appear. */
        bool all_ones = ((k & (k + 1)) == 0);    /* true if k == 2^m-1 */

        bool has0 = false;
        bool has1 = false;
        idx_t tmp = k;
        while (tmp > 0 && (all_ones ? !has1 : !(has0 && has1))) {
            int d = (int)(tmp % 4);
            if (d == 0) has0 = true;
            if (d == 1) has1 = true;
            tmp /= 4;
        }

        bool ok = false;
        if (all_ones) {
            ok = has1;          /* need at least one '1' digit */
        } else {
            ok = has0 && has1;  /* need both '0' and '1' digits */
        }

        if (ok) {
            /* Ensure enough capacity in the cache array */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) return 0;           /* allocation failed */
                cache      = new_cache;
                cache_cap  = new_cap;
            }
            cache[cache_size++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}

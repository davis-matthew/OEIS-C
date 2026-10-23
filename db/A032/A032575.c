/*
 * A032575  Numbers k whose decimal representation, read as a base-22 value
 *          and divided by k, yields an integer.
 * https://oeis.org/A032575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032575: Numbers k such that the base‑22 value of the decimal
   representation of k divided by k is an integer.  The function
   generates the sequence on demand and caches already computed terms
   so that any call order is supported. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/*  a(idx_t n) returns the n-th term (n>=1).   */
seq_t a(idx_t n)
{
    /* static cache for already found terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static idx_t  sz    = 0;   /* number of terms stored */
    static idx_t  last_k = 0;  /* last k examined */

    if (n == 0) return 0;      /* offset 1 – behaviour for n<1 is irrelevant */

    /* ensure cache has at least n entries */
    while (sz < n) {
        last_k++;                   /* next candidate number */

        /* convert last_k to its decimal digits (reversed order) */
        idx_t temp = last_k;
        unsigned digits[20];        /* enough for 20 decimal digits */
        int dlen = 0;
        if (temp == 0) {
            digits[0] = 0;
            dlen = 1;
        } else {
            while (temp > 0) {
                digits[dlen++] = (unsigned)(temp % 10);
                temp /= 10;
            }
        }

        /* compute the base‑22 value of those digits */
        seq_t val = 0;
        for (int i = dlen - 1; i >= 0; --i)
            val = val * 22 + digits[i];

        /* test divisibility: val % last_k == 0 ? */
        seq_t sk = (seq_t)last_k;
        if ((val % sk) == 0) {
            /* store the term in the cache */
            if (sz == cap) {
                size_t newcap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) { /* allocation failed – abort gracefully */
                    free(cache);
                    cache = NULL;
                    cap   = 0;
                    sz    = 0;
                    return 0;
                }
                cache = tmp;
                cap   = newcap;
            }
            cache[sz++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];  /* zero‑based index in cache */
}

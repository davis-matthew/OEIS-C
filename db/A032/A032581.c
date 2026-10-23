/*
 * A032581  Numbers k such that A174141(k) is divisible by k.
 * https://oeis.org/A032581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 42, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032581: numbers k such that the decimal representation of k
   read as a base‑25 number is divisible by k.
   The function returns the nth term by brute‑force search
   and memoises found terms.  The cache is static so calls in any
   order are handled correctly. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of terms  */
    static seq_t *cache   = NULL;
    static size_t  count  = 0;   /* number of cached terms   */
    static size_t  cap    = 0;   /* capacity of cache       */
    static idx_t   last_k = 0;   /* largest k already processed */

    if (n == 0) {            /* OEIS offset starts at 1; undefined for 0 */
        return 0;
    }

    /* If the required term is already cached, return it. */
    if (n <= count) {
        return cache[n - 1];
    }

    /* Ensure cache is large enough. */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < (size_t)n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – behaviour undefined in this context. */
            return 0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Search for further k values until we have at least n terms. */
    for (idx_t k = last_k + 1; ; ++k) {
        idx_t rem = 0;
        idx_t pow = 1 % k;   /* 1 modulo k */

        /* Process decimal digits from least significant to most. */
        idx_t tmp = k;
        while (tmp > 0) {
            unsigned digit = (unsigned)(tmp % 10);
            rem = (rem + (idx_t)(((__int128)pow * digit) % k)) % k;
            pow = (idx_t)(((__int128)pow * 25) % k);
            tmp /= 10;
        }

        if (rem == 0) {
            /* k satisfies the divisibility condition – store it. */
            cache[count++] = (seq_t)k;
            if (count == n) {
                last_k = k;
                break;
            }
        }
    }

    return cache[n - 1];
}

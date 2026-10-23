/*
 * A044088  Numbers k such that string 2,1 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2780, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the n‑th integer k such that the base‑5 representation
   of k contains the consecutive digits 2,1 but the representation of k−1 does
   not.  Numbers are returned in increasing order. */
#include <stdbool.h>
#include <stdlib.h>

/* Helper: true if base‑5 digits of x contain the substring “21”.  */
static inline bool contains21(seq_t x)
{
    seq_t prev = 5;           /* sentinel larger than any base‑5 digit */
    while (x > 0) {
        seq_t digit = x % 5;
        /* In the least‑significant‑digit-first order the pair ...21
           appears as d=1 followed by d=2, hence we test for digit==2
           with the previously seen digit==1. */
        if (digit == 2 && prev == 1)
            return true;
        prev = digit;
        x /= 5;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* storage for found k’s          */
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;       /* number of terms stored so far */
    static seq_t next_k      = 1;       /* next integer to examine       */

    /* initialise cache once */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure */
    }

    /* generate terms until we have at least n elements */
    while (cache_len < (size_t)n) {
        seq_t k = next_k++;
        if (contains21(k) && !contains21(k - 1)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
    }

    return cache[n - 1];   /* n is 1‑based */
}

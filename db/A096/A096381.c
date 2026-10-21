/*
 * A096381  Beginning with 2, 7, multiply successive pairs of members and
 *          adjoin the result as the next one or two members of the sequence,
 *          depending on whether the product is a one- or two-digit number.
 * https://oeis.org/A096381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A096381 – successive product digit sequence
   a(1) = 2, a(2) = 7.
   For i ≥ 1, let p = a(i) * a(i+1).
   Append the decimal digits of p (from most to least significant)
   as the next term(s) of the sequence.
   The sequence is produced lazily and memoised.
   The function a(n) returns the n‑th term for any n ≥ 1. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static state: cache of terms, its length and capacity,
       and the index of the next 2‑term pair to multiply. */
    static seq_t *cache      = NULL;   /* array of computed terms   */
    static size_t cache_len  = 0;      /* number of terms stored   */
    static size_t cache_cap  = 0;      /* current allocation cap   */
    static size_t pair_i     = 0;      /* 0‑based index of first
                                         term of the next pair */

    if (n == 0)                       /* undefined, but safe */
        return (seq_t)0;

    /* Initialise cache with the two fixed starting terms. */
    if (cache_len == 0) {
        /* Allocate a modest initial capacity. */
        cache_cap = 16;
        cache     = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache)                /* failed allocation */
            return (seq_t)0;
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)7;
        cache_len = 2;
        pair_i = 0;
    }

    /* Grow the cache while we still lack the requested term. */
    while (cache_len < (size_t)n) {
        /* product of the current pair (always fits in seq_t) */
        seq_t prod = cache[pair_i] * cache[pair_i + 1];

        /* convert prod (0‑81) into its decimal digits */
        unsigned char d[2];
        size_t dlen = 0;

        if (prod < 10) {                 /* single digit */
            d[0] = (unsigned char)prod;
            dlen = 1;
        } else {                         /* two digits: 10‑81 */
            d[0] = (unsigned char)(prod / 10);
            d[1] = (unsigned char)(prod % 10);
            dlen = 2;
        }

        /* append digits to the cache, resizing if needed */
        for (size_t k = 0; k < dlen; ++k) {
            if (cache_len >= cache_cap) {
                /* double the capacity, but not less than 16 */
                size_t new_cap = cache_cap * 2;
                if (new_cap < 16) new_cap = 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp)                      /* allocation failed */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = d[k];
        }

        /* advance to the next pair for the next iteration */
        ++pair_i;
    }

    return cache[n - 1];
}

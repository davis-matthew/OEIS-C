/*
 * A059095  List consisting of the representation of n in base 3 using digits
 *          -1, 0, 1.
 * https://oeis.org/A059095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   a(n) = the n‑th digit (1‑based) of the concatenation of the
 *            balanced ternary representations of 1,2,3,… Each integer k is
 *            written in balanced ternary with digits in {−1,0,1} and its
 *            digits are appended in most‑significant‑digit‑first order to an
 *            infinite sequence. a(n) is that sequence’s n‑th term
 * Range:     i8 n <= 4560, i16 n <= 4560, i32 n <= 4560, i64 n <= 4560,
 *            i128 n <= 4560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Return the n‑th digit of the concatenated balanced–ternary word.  */
seq_t a(idx_t n)
{
    /* Static cache of digits already generated. */
    static seq_t *cache = NULL;
    static size_t cache_len  = 0;   /* number of elements stored */
    static size_t cache_cap  = 0;   /* capacity of the buffer      */
    static idx_t next_num    = 1;   /* next integer to convert    */

    if (n == 0)          /* offset is 1; anything below that is undefined */
        return (seq_t)0;

    /* Ensure we have at least n digits in the cache. */
    while (cache_len < (size_t)n) {
        /* Convert next_num to balanced ternary, collecting digits
           in an intermediate fixed array (max 64 digits). */
        idx_t x = next_num;
        seq_t tmp[64];
        size_t d = 0;

        /* Special case for 0 – not needed because next_num>=1. */
        if (x == 0) {
            tmp[d++] = (seq_t)0;
        } else {
            while (x != 0) {
                int r = (int)(x % 3);   /* r = 0,1,2 */
                x /= 3;
                if (r == 2) {           /* 2 → –1 with a carry */
                    r = -1;
                    ++x;
                }
                tmp[d++] = (seq_t)r;     /* low‑to‑high order */
            }
        }

        /* Grow cache if necessary. */
        if (cache_len + d > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            while (new_cap < cache_len + d)
                new_cap *= 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) {
                /* Allocation failed – abort with undefined behaviour
                   (not expected in test harness). */
                exit(EXIT_FAILURE);
            }
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Append the digits in most‑significant‑first order. */
        for (size_t i = d; i > 0; --i)
            cache[cache_len++] = tmp[i - 1];

        ++next_num;
    }

    return cache[n - 1];
}

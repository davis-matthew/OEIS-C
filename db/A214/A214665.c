/*
 * A214665  The y-coordinates of prime numbers in an Ulam spiral oriented
 *          counterclockwise with first step east.
 * https://oeis.org/A214665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6521, i16 n <= 9359, i32 n <= 9359, i64 n <= 9359,
 *            i128 n <= 9359, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = y‑coordinate of the nth prime in the Ulam spiral
   oriented counter‑clockwise with first step east. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* cache of y‑coordinates of seen primes */
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;

    /* state of the spiral walk */
    static idx_t cur_num = 1;          /* number at current position */
    static int64_t x = 0, y = 0;       /* coordinates of current position */
    static int dir = 0;                /* 0=E,1=N,2=W,3=S */
    static int step_len = 1;           /* steps to take in current leg */
    static int steps_taken = 0;        /* steps taken in current leg */
    static int legs_done = 0;          /* legs finished with current step_len */

    /* initialise cache if necessary */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, return 0 as sentinel */
    }

    /* generate further primes until we have at least n of them */
    while (cache_size < n) {
        /* move to next cell in the spiral */
        cur_num++;

        int64_t dx = 0, dy = 0;
        switch (dir) {
            case 0: dx = 1; break;   /* east  */
            case 1: dy = 1; break;   /* north */
            case 2: dx = -1; break;  /* west  */
            case 3: dy = -1; break;  /* south */
        }
        x += dx;
        y += dy;
        steps_taken++;

        /* if the current cell contains a prime, cache its y‑coordinate */
        if (is_prime(cur_num)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)y;
        }

        /* advance direction and possibly increase step length */
        if (steps_taken == step_len) {
            dir = (dir + 1) & 3;
            steps_taken = 0;
            legs_done++;
            if (legs_done == 2) {
                legs_done = 0;
                step_len++;
            }
        }
    }

    return cache[n - 1];
}

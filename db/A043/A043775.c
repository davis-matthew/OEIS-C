/*
 * A043775  Numbers n such that number of runs in base 3 representation of n
 *          is congruent to 2 mod 3.
 * https://oeis.org/A043775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A043775: numbers n such that the number of runs in the base‑3
 * representation of n is congruent to 2 modulo 3.
 *
 * The function generates the terms by scanning natural numbers,
 * counting base‑3 runs, and storing those that satisfy the congruence.
 * A static cache of the found terms is maintained so that repeated
 * calls are efficient.  The implementation works for ordinary
 * unsigned integer types as well as for arbitrary‑precision classes
 * (the seq_t type supplied by the build system).
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next natural number to test */
    static seq_t next_val = 1;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t num = next_val;
        int runs = 0;
        int prev_digit = -1;

        /* count runs in base‑3 representation of next_val */
        while (num > (seq_t)0) {
            int digit = (int)(num % (seq_t)3);
            if (digit != prev_digit) {
                ++runs;
                prev_digit = digit;
            }
            num /= (seq_t)3;
        }

        if (runs % 3 == 2) {
            /* store the term in the cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                /* cast for C++ compatibility */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_val;
        }

        ++next_val;  /* proceed to next natural number */
    }

    return cache[(size_t)n - 1];
}

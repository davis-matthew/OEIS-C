/*
 * A158750  Primes that are the concatenation of the first k triangular
 *          numbers for some k.
 * https://oeis.org/A158750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, bref, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence of primes that are the concatenation of the first k
   triangular numbers (k ≥ 1).  The function searches k increasing,
   concatenates T(1)…T(k), tests for primality, and stores the found
   primes.  It memoises the results, so any call to a(n) works in
   O(n) time overall.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"        /* seq_t pow10(idx_t e);          */
#include "../utils/digit_len.h"    /* int digit_len(seq_t x);        */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n);    */

seq_t a(idx_t n)
{
    if (n == 0) {          /* offset is 1; undefined for n==0   */
        return 0;
    }

    /* Static state to avoid recomputation between calls */
    static idx_t next_k = 0;                /* next triangular index to process */
    static seq_t cur_concat = 0;            /* concatenation of T(1)…T(k) */
    static seq_t *cache = NULL;             /* stored primes */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Generate primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Advance to the next triangular number */
        next_k++;

        /* T(next_k) = next_k*(next_k+1)/2  (cast to seq_t for precision) */
        seq_t t = (seq_t)next_k * ((seq_t)next_k + 1) / 2;

        /* New concatenation: cur_concat * 10^d + t  where d = digit_len(t) */
        int d = digit_len(t);
        seq_t pow10d = pow10((idx_t)d);
        cur_concat = cur_concat * pow10d + t;

        /* Store if the new concatenation is prime */
        if (is_prime_big(cur_concat)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {            /* Allocation failed – abort */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur_concat;
        }
    }

    return cache[n - 1];
}

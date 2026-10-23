/*
 * A120227  Numbers k such that 5+k and 5*k+1 are prime.
 * https://oeis.org/A120227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 516, u32 n <= 517, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 5+k and 5*k+1 are prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

#ifdef __cplusplus
#include <vector>
#endif

/* Return the n-th element (n >= 1) of OEIS sequence A120227. */
seq_t a(idx_t n)
{
    /* Maximum index already computed. */
    static idx_t max_n = 0;

    /* In C++ we use std::vector<seq_t> for automatic resizing.
       In C we manage a dynamically allocated array manually. */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static seq_t next_k = 2;

    /* Ensure we have at least n terms stored. */
    while (cache.size() < (size_t)n) {
        /* Find the next k satisfying the primality conditions. */
        while (true) {
            seq_t k = next_k;
            next_k = k + 1;                 /* next candidate start point       */
            seq_t sum = k + ((seq_t)5);     /* 5 + k                            */
            seq_t expr = k * ((seq_t)5) + 1; /* 5*k + 1                          */
            if (is_prime_big(sum) && is_prime_big(expr)) {
                cache.push_back(k);         /* store the found term              */
                break;                       /* exit the inner search loop        */
            }
        }
    }
    return cache[(size_t)n - 1];
#else
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static seq_t next_k = 2;       /* next candidate value to test */

    /* Grow the cache as needed. */
    if (cache_len < (size_t)n) {
        /* Allocate space for at least n terms. */
        size_t needed = (size_t)n;
        if (cache_cap < needed) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Find new terms until we have n of them. */
        while (cache_len < (size_t)n) {
            /* Search for the next k satisfying the primality conditions. */
            for (;;) {
                seq_t k = next_k;
                next_k = k + 1;                 /* next starting point   */
                seq_t sum = k + ((seq_t)5);     /* 5 + k                */
                seq_t expr = k * ((seq_t)5) + 1;/* 5*k + 1              */
                if (is_prime_big(sum) && is_prime_big(expr)) {
                    cache[cache_len++] = k;   /* store the found term   */
                    break;                    /* exit the inner loop    */
                }
                /* continue searching */
            }
        }
    }

    return cache[(size_t)n - 1];
#endif
}

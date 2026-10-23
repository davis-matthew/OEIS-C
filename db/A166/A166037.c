/*
 * A166037  Numbers that are the sum of 2 successive nonprimes A141468.
 * https://oeis.org/A166037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A141468(n) + A141468(n+1)
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
#  include <new>
#endif
#include "../utils/is_prime.h"

/* Static cache of the sequence of non‑prime numbers
   (A141468): 0,1,4,6,8,9,10,12,...
   We store them in order starting with index 0. */
static seq_t *nonprimes = NULL;
static idx_t nonprimes_len = 0;
static idx_t nonprimes_capacity = 0;
static idx_t next_candidate = 2;

/* Ensure that `nonprimes` contains at least `needed` elements */
static void ensure_nonprimes(idx_t needed)
{
    if (nonprimes_len >= needed)
        return;

    /* One‑time initialization of the cache with the first two
       non‑prime numbers 0 and 1. */
    if (!nonprimes) {
#ifdef __cplusplus
        nonprimes_capacity = 256;
        nonprimes = new seq_t[nonprimes_capacity];
#else
        nonprimes_capacity = 256;
        nonprimes = (seq_t *)malloc(sizeof(seq_t) * nonprimes_capacity);
#endif
        if (!nonprimes)
            exit(1);
        nonprimes[0] = (seq_t)0;
        nonprimes[1] = (seq_t)1;
        nonprimes_len = 2;
        next_candidate = 2;
    }

    /* Generate additional non‑prime numbers until we have enough. */
    for (;;) {
        if (!is_prime(next_candidate)) {
            if (nonprimes_len >= nonprimes_capacity) {
#ifdef __cplusplus
                nonprimes_capacity *= 2;
                seq_t *tmp = new seq_t[nonprimes_capacity];
                for (size_t i = 0; i < nonprimes_len; ++i)
                    tmp[i] = nonprimes[i];
                delete[] nonprimes;
                nonprimes = tmp;
#else
                nonprimes_capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(nonprimes,
                                              sizeof(seq_t) * nonprimes_capacity);
                if (!tmp)
                    exit(1);
                nonprimes = tmp;
#endif
            }
            nonprimes[nonprimes_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
        if (nonprimes_len >= needed)
            break;
    }
}

/* Main function that returns the nth term of a(n) (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                           /* undefined, but safe */
    idx_t needed = n + 1;                           /* need elements 0..n */
    ensure_nonprimes(needed);
    return nonprimes[n - 1] + nonprimes[n];
}

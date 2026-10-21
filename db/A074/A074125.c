/*
 * A074125  Group the composite numbers so that the sum of the n-th group is a
 *          multiple of the n-th prime: (4), (6), (8, 9, 10, 12, 14, 15, 16,
 *          18, 20, 21, 22), (24, 25), (26, 27, 28, 30, 32), (33, 34, ...),
 *          ... Sequence gives the first term of each group.
 * https://oeis.org/A074125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequentially group composite numbers so that the sum
 * of the n-th group is a multiple of the n-th prime; returns the
 * first composite of each group (a(1)=4). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, value unused */

    /* static state for memoisation */
    static bool initialized = false;
    static seq_t *firsts = NULL;
    static size_t firsts_len = 0, firsts_cap = 0;
    static seq_t curr = 0;          /* next candidate integer to consider */

    if (!initialized) {
        /* initialisation */
        firsts_cap = 64;
        firsts = (seq_t *)malloc(firsts_cap * sizeof(seq_t));
        firsts_len = 0;
        curr = 4; /* first composite number */
        initialized = true;
    }

    while (firsts_len < n) {
        /* skip any primes to find the next composite */
        while (is_prime_small(curr)) curr++;

        /* start a new group with current composite */
        firsts[firsts_len++] = curr;

        /* grow array if necessary */
        if (firsts_len > firsts_cap) {
            firsts_cap <<= 1;
            firsts = (seq_t *)realloc(firsts, firsts_cap * sizeof(seq_t));
        }

        /* prime for this group (1‑based index) */
        seq_t prime = (seq_t)get_prime((idx_t)firsts_len);

        /* accumulate sum until divisible by the prime */
        seq_t sum = 0;
        seq_t c = curr;
        do {
            sum += c;
            c++;
            while (is_prime_small(c)) c++; /* skip primes inside the group */
        } while (sum % prime != 0);

        /* next group starts at next composite after the last added */
        curr = c;
    }

    return firsts[n - 1];
}

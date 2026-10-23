/*
 * A104718  Concatenations of pairs of primes that differ by 8.
 * https://oeis.org/A104718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(A023202(n), A023202(n)+8)
 * Range:     u8 none, u16 n <= 9, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                            where A023202(n) is the n-th prime p such that p+8
 *                            is also prime                                               */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/pow10.h"      /* seq_t pow10(idx_t exp); */
#include "../utils/digit_len.h"  /* idx_t digit_len(seq_t x); */

/* static state for memoization */
static bool built = false;
static size_t build_limit = 100000;
static uint64_t *pair_primes = NULL;
static size_t pair_count = 0;
static size_t pair_cap   = 0;

/* ------------------------------------------------------------------ */
/* Utility: extend the sieve until we have at least 'needed' prime
   pairs where p+8 is also prime.                                 */
/* ------------------------------------------------------------------ */
static void build_pairs_until(size_t needed)
{
    if (built && pair_count >= needed)
        return;

    size_t limit = build_limit;
    while (1) {
        /* simple Eratosthenes sieve */
        char *is_prime = (char *)malloc(limit + 1);
        if (!is_prime)
            abort();               /* memory allocation failed */
        memset(is_prime, 1, limit + 1);
        is_prime[0] = is_prime[1] = 0;
        for (size_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* rebuild pairs */
        free(pair_primes);
        pair_primes = NULL;
        pair_count = 0;
        pair_cap = 0;

        for (size_t p = 2; p + 8 <= limit; ++p) {
            if (is_prime[p] && is_prime[p + 8]) {
                if (pair_count == pair_cap) {
                    pair_cap = pair_cap ? pair_cap * 2 : 16;
                    uint64_t *tmp = (uint64_t *)realloc(pair_primes,
                                                        pair_cap * sizeof(uint64_t));
                    if (!tmp)
                        abort();   /* memory allocation failed */
                    pair_primes = tmp;
                }
                pair_primes[pair_count++] = p;
            }
        }

        free(is_prime);

        if (pair_count >= needed) {
            built = true;
            build_limit = limit;
            break;
        }
        /* need more primes, double the search limit */
        limit *= 2;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function requested by problem statement                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)    /* behavior below offset is irrelevant */
        return (seq_t)0;

    build_pairs_until((size_t)n);

    /* safety check in case something went wrong */
    if (n > pair_count)
        abort();

    uint64_t p = pair_primes[(size_t)n - 1];
    seq_t q = (seq_t)(p + 8);
    idx_t d = digit_len(q);
    seq_t pow10d = pow10(d);
    return (seq_t)p * pow10d + q;
}

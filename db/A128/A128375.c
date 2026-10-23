/*
 * A128375  Palindromic primes that start and end with 9.
 * https://oeis.org/A128375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th palindromic prime that starts and ends
 * with the digit 9.  It is generated incrementally by constructing
 * odd‑length palindromes whose first and last digit are 9, testing each
 * for primality with is_prime_big, and storing the primes found in a
 * static cache.  Subsequent calls reuse previously computed terms. */
/* Note: Indexing follows OEIS: a(1) is the first term. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/append_candidate.h"   /* append_candidate() */
#include "../utils/pow10.h"              /* pow10() */
#include "../utils/is_prime_big.h"       /* is_prime_big() */

seq_t a(idx_t n)
{
    /* Cache of primes that have been found so far. */
    static seq_t *prime_cache   = NULL;
    static size_t cache_len     = 0;
    static size_t cache_cap     = 0;

    /* State of the incremental palindrome generator. */
    static bool   initialized   = false;
    static int    cur_len       = 3;      /* current odd length (>=3)   */
    static int    k             = 1;      /* k = (cur_len-1)/2           */
    static seq_t  powk          =  0;      /* 10^k                        */
    static seq_t  left_start    =  0;      /* 9 * 10^k                    */
    static idx_t  d             =  0;      /* current offset within left   */
    static idx_t  max_d         =  0;      /* 10^k (type idx_t)           */

    /* Initialise generator on first call. */
    if (!initialized) {
        powk      = pow10((idx_t)k);
        left_start= 9 * powk;
        max_d     = (idx_t)powk;
        initialized = true;
    }

    /* Generate further primes until we have at least n of them. */
    while (cache_len < n) {
        seq_t pal;

        /* Find the next palindrome prime. */
        while (true) {
            /* Advance to the next length if we have exhausted all left
             * prefixes of the current length. */
            if (d >= max_d) {
                cur_len += 2;
                k = (cur_len - 1) / 2;
                powk      = pow10((idx_t)k);
                left_start= 9 * powk;
                max_d     = (idx_t)powk;
                d         = 0;
            }

            /* Construct the palindrome from the current left part. */
            seq_t left = left_start + (seq_t)d;
            d++;

            pal = left;
            seq_t t = left / 10;
            while (t > 0) {
                pal = pal * 10 + (t % 10);
                t   = t / 10;
            }

            /* Accept it if it is prime. */
            if (is_prime_big(pal)) {
                append_candidate(&prime_cache, &cache_len, &cache_cap, pal);
                break;
            }
        }
    }

    return prime_cache[n - 1];
}

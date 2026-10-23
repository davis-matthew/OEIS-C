/*
 * A046491  Palindromes expressible as the sum of 3 consecutive palindromic
 *          primes.
 * https://oeis.org/A046491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   is_palindrome, is_prime_big, make_pal_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046491: Palindromes expressible as the sum of 3 consecutive palindromic primes
   Implementation: generate palindromic numbers in ascending order, test primality,
   keep a sliding window of the last three pal primes, compute their sum and test
   whether the sum is a base‑10 palindrome.  The sequence is built on demand and
   cached, so any n≥1 can be answered in O(1) after the necessary terms have been
   generated.  The algorithm uses integer arithmetic only and relies on the
   provided helper functions for primality testing and palindrome checking.  */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/is_palindrome.h"
#include "../utils/make_pal_uint64.h"

/* Convert an exponent e to 10^e (e≥0).  */
static uint64_t pow10_uint64(int e)
{
    uint64_t r = 1;
    while (e-- > 0)
        r *= 10;
    return r;
}

/* Sequence entry a(n).  */
seq_t a(idx_t n)
{
    /* Static cache of computed sequence values.  */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;
    static idx_t  len     = 0;

    /* Sliding window for the last three palindromic primes.  */
    static uint64_t lastp[3]  = {0, 0, 0};
    static idx_t    prime_cnt = 0;

    /* Current palindrome generation state.  */
    static int      curr_len = 1;       /* Current digit length.  */
    static uint64_t prefix   = 0;       /* Current prefix value.  */
    static uint64_t upper    = 0;       /* Upper bound for the current prefix.  */

    /* One‑time initialization.  */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return 0;                   /* Allocation failure – unlikely.  */
        /* Initialise palindrome generator for 1‑digit palindromes.  */
        curr_len = 1;
        prefix   = 1;
        upper    = 9;
    }

    /* Generate terms until we have at least n of them.  */
    while (len < n) {
        /* Produce the next palindromic number.  */
        uint64_t pal = make_pal_uint64(prefix, curr_len % 2 == 1);

        /* Test whether it is prime.  */
        if (is_prime_big((seq_t)pal)) {
            /* Update sliding window.  */
            lastp[0] = lastp[1];
            lastp[1] = lastp[2];
            lastp[2] = pal;
            prime_cnt++;

            /* Only start forming triples after we have 3 primes.  */
            if (prime_cnt >= 3) {
                uint64_t sum = lastp[0] + lastp[1] + lastp[2];
                if (is_palindrome((seq_t)sum, 10)) {
                    /* Ensure the cache has enough space.  */
                    if (len == cap) {
                        cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                        if (!tmp) {
                            free(cache);
                            return 0;   /* Allocation failure.  */
                        }
                        cache = tmp;
                    }
                    cache[len++] = (seq_t)sum;
                }
            }
        }

        /* Advance to the next palindrome path.  */
        prefix++;
        if (prefix > upper) {          /* End of current length.  */
            curr_len++;
            int half_len = (curr_len + 1) / 2;
            prefix   = pow10_uint64(half_len - 1);
            upper    = pow10_uint64(half_len) - 1;
        }
    }

    return cache[n - 1];
}

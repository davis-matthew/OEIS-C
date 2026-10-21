/*
 * A225105  Odd numbers n such that the largest odd term in Collatz(3x+1)
 *          trajectory of n is prime.
 * https://oeis.org/A225105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 474, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth odd integer n>0 such that the largest odd term in
   the Collatz(3x+1) trajectory of n is a prime.  a(1) = 3, a(2) = 5, … */

/*  Function uses the generic isPrimeGeneric routine to test primality.
    The cache holds all qualifying odd numbers found so far; it grows
    automatically as needed.  All arithmetic is performed on seq_t. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

static bool max_odd_prime_is_prime(seq_t start)
{
    seq_t cur   = start;
    seq_t maxodd = start;            /* start is odd */

    while (cur != (seq_t)1) {
        if (cur & (seq_t)1)          /* odd: 3x+1, becomes even */
            cur = (seq_t)3 * cur + (seq_t)1;
        else                         /* even: halve */
            cur = cur >> 1;

        if (cur & (seq_t)1) {        /* new odd value could be new maximum */
            if (cur > maxodd)
                maxodd = cur;
        }
    }
    return isPrimeGeneric(maxodd);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;            /* stored qualifying odds   */
    static size_t cache_len = 0;           /* number of terms stored   */
    static size_t cache_cap = 0;           /* allocated capacity       */
    static seq_t last_candidate = 1;       /* generates next odd 3,5,… */

    if (n == 0)
        return (seq_t)0;                    /* undefined – harmless */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        last_candidate += (seq_t)2;         /* next odd */

        if (max_odd_prime_is_prime(last_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_candidate;
        }
    }
    return cache[n - 1];
}

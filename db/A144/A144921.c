/*
 * A144921  Duplicate of A002476.
 * https://oeis.org/A144921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p ≡ 1 (mod 6) – i.e. primes of the form 6*k+1 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* declaration for is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* a(1) = 7, a(2) = 13, …  */
    if (n == 0)
        return 0;   /* value below the offset is undefined */

    /* Cache for the primes of the form 6k+1 found so far. */
    static seq_t *list = NULL;
    static size_t len   = 0;     /* number of primes stored */
    static size_t cap   = 0;     /* allocated capacity */

    /* Next candidate of the form 6k+1.  Start at 7. */
    static seq_t next = (seq_t)7;

    /* Initialise the cache on the first call. */
    if (len == 0) {
        cap   = 16;
        list  = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate new primes until we have at least n of them. */
    while (len < (size_t)n) {
        /* If the candidate is prime, store it. */
        if (is_prime_big(next)) {
            if (len == cap) {
                cap = cap ? cap * 2 : 16;
                list = (seq_t *)realloc(list, cap * sizeof(seq_t));
            }
            list[len++] = next;
        }
        /* Next candidate of the form 6k+1. */
        next += 6;
    }

    return list[(size_t)n - 1];
}

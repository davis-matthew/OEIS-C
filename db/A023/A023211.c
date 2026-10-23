/*
 * A023211  Primes p such that 3*p + 10 is also prime.
 * https://oeis.org/A023211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 718, u32 n <= 853, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 3p+10 is also prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

/* Return the n‑th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Static storage for memoised values. */
    static seq_t *list     = NULL;  /* array storing the qualifying primes */
    static size_t sz       = 0;     /* number of primes stored so far  */
    static size_t cap      = 0;     /* current allocation capacity         */
    static uint64_t last   = 2;     /* last tested integer (even initially) */

    /* First invocation: initialise dynamic array. */
    if (list == NULL) {
        cap = 64;
        list = (seq_t *)malloc(cap * sizeof(seq_t));
        /* begin search just before the first odd prime 3 */
        last = 2;
    }

    /* Generate terms until the desired index is reached. */
    while (sz < n) {
        /* Advance to the next odd candidate. */
        if (last < 3)
            last = 3;
        else
            last += 2;

        /* Test primality of the candidate. */
        if (!is_prime(last))
            continue;

        /* Compute 3*last + 10 without overflow. */
        uint64_t tmp = 3 * last + 10ULL;
        seq_t q = (seq_t)tmp;

        /* Test primality of 3*last+10. */
        if (!is_prime_big(q))
            continue;

        /* Store the qualifying prime; grow storage if necessary. */
        if (sz == cap) {
            cap *= 2;
            list = (seq_t *)realloc(list, cap * sizeof(seq_t));
        }
        list[sz++] = (seq_t)last;
    }

    /* Return the asked term (1‑based index). */
    return list[n - 1];
}

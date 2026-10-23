/*
 * A156007  Primes p such that 32*p + 93 is also prime.
 * https://oeis.org/A156007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 155, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A156007: the n‑th prime p such that 32*p+93 is also prime.
   The function builds the list on demand, storing the qualifying primes
   in a static array and expanding it as necessary. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static seq_t *values = NULL;   /* cached primes satisfying 32p+93 prime */
    static size_t count = 0;       /* number of stored primes */
    static size_t capacity = 0;    /* allocated capacity of values */
    static uint64_t candidate = 2; /* current integer candidate */

    /* Expand the cache until we have at least n primes */
    while (count < n) {
        seq_t cand = (seq_t)candidate;          /* convert to seq_t for primality test */

        if (is_prime_big(cand)) {          /* first prime check */
            seq_t q = cand * (seq_t)32 + (seq_t)93;  /* compute 32*p + 93 */
            if (is_prime_big(q)) {          /* second prime check */
                /* Grow the array if full */
                if (count >= capacity) {
                    size_t new_cap = capacity ? capacity * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* allocation failure */
                    values = tmp;
                    capacity = new_cap;
                }
                values[count++] = cand;      /* store the qualifying prime */
            }
        }

        /* Advance to next candidate: skip even numbers after 2 */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }

    return values[n - 1];
}

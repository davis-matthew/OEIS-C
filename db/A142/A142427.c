/*
 * A142427  Primes congruent to 16 mod 49.
 * https://oeis.org/A142427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes congruent to 16 modulo 49 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static storage for the computed primes */
    static seq_t *vals = NULL;
    static size_t count = 0;   /* number of primes stored */
    static size_t cap   = 0;   /* allocated capacity */
    static uint64_t last_checked = 16; /* last number examined */

    /* The sequence starts at index 1, so n==0 is an error case. */
    if (n == 0) return 0;

    /* Grow the array until it can hold n entries. */
    if (count < (size_t)n) {
        if (cap < (size_t)n) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < (size_t)n) new_cap = n;
            /* cast from void* to seq_t* so the code is valid in C++ */
            seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
            vals = tmp;
            cap = new_cap;
        }

        /* Find successive primes until we have enough. */
        while (count < (size_t)n) {
            last_checked = next_prime(last_checked + 1);
            if ((last_checked % 49ULL) == 16ULL) {
                vals[count++] = (seq_t)last_checked;
            }
        }
    }

    return vals[(size_t)n - 1];
}

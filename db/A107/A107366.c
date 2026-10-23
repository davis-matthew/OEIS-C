/*
 * A107366  Numbers k such that 101*k + 103 is prime.
 * https://oeis.org/A107366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 4488, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth non‑negative integer k such that 101*k + 103 is prime */

/* This function incrementally searches k (starting at 0), tests
 * 101*k+103 for primality using the supplied is_prime(uint64_t) helper
 * and stores the qualifying k values in a static array.  The array is
 * extended as necessary, so multiple calls to a(n) with arbitrary
 * order of n stay fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static storage for found k values */
    static seq_t *kvals = NULL;
    static size_t  count = 0;
    static size_t  capacity = 0;
    static idx_t   next_k = 0;   /* next k to test */

    /* initial allocation */
    if (kvals == NULL) {
        capacity = 1024;
        kvals = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (kvals == NULL)
            return 0; /* allocation failure -> zero placeholder */
    }

    /* extend cache until it contains at least n terms */
    while (count < n) {
        /* test candidate 101*k + 103 for primality */
        uint64_t candidate = (uint64_t)101 * (uint64_t)next_k + 103ULL;
        if (is_prime(candidate)) {
            /* ensure enough space */
            if (count == capacity) {
                capacity <<= 1;
                seq_t *tmp = (seq_t *)realloc(kvals, capacity * sizeof(seq_t));
                if (tmp == NULL)
                    break; /* allocation failure, return last value */
                kvals = tmp;
            }
            kvals[count++] = (seq_t)next_k;  /* store the k itself */
        }
        next_k++;
    }

    return kvals[n - 1];
}

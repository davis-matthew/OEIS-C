/*
 * A127260  Indices n of odd numbers of the form 8*n+1 that are not primes.
 * https://oeis.org/A127260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the (n-1)-th integer k (k ≥ 0) for which 8*k+1 is not a prime.
   Computed by testing successive values 8*k+1 for compositeness. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static idx_t *indices = NULL;  /* stored k values */
    static size_t sz = 0;          /* number of stored values */
    static size_t cap = 0;         /* allocated capacity */
    static idx_t next = 0;         /* next k to test */

    /* Ensure we have computed at least n values. */
    while (sz < (size_t)n) {
        idx_t val = 8 * next + 1;
        if (!is_prime(val)) {
            if (sz == cap) {
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                idx_t *tmp = (idx_t *)realloc(indices, new_cap * sizeof(idx_t));
                if (!tmp) {
                    exit(EXIT_FAILURE);  /* no memory */
                }
                indices = tmp;
                cap = new_cap;
            }
            indices[sz++] = next;
        }
        ++next;
    }

    return (seq_t)indices[n - 1];
}

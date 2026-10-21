/*
 * A230117  Primes p such that 2*p+1 is prime and 2*p+3 is not prime.
 * https://oeis.org/A230117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 372, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Variant: a(n) = the n-th prime p such that 2*p+1 is prime and 2*p+3 is composite. */

#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>  /* bool, true, false */

#include "../utils/is_prime.h"   /* is_prime(idx_t) */

extern seq_t a(idx_t n)
{
    /* Array of terms already found.  Terms are stored 0‑based; a(1) is at arr[0]. */
    static seq_t *arr = NULL;
    static idx_t  cap   = 0;        /* current capacity of arr */
    static idx_t  size  = 0;        /* number of terms stored so far */
    static idx_t  cand  = 3;        /* last candidate tested (odd number) */

    /* If n is less than the previously computed size, just return it. */
    if (n >= 1 && n <= size)
        return arr[n - 1];

    /* Grow the array if necessary. */
    if (cap == 0) {
        cap = 256;
        arr = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    if (n > cap) {
        while (cap < n) cap <<= 1;            /* double until big enough */
        arr = (seq_t *)realloc(arr, cap * sizeof(seq_t));
    }

    /* Generate new terms until we have at least n of them. */
    while (size < n) {
        /* Find next prime candidate.  Skip even numbers. */
        while (!is_prime(cand)) cand += 2;

        /* cand is prime.  Check the two derived numbers. */
        seq_t q = (seq_t)cand * 2 + 1;
        if (is_prime((idx_t)q)) {
            seq_t r = (seq_t)cand * 2 + 3;
            if (!is_prime((idx_t)r)) {
                arr[size++] = cand;          /* store the term */
            }
        }

        /* Advance to the next odd number. */
        cand += 2;
    }

    return arr[n - 1];
}

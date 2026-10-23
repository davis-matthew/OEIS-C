/*
 * A111326  Numbers k such that 12 divides prime(1) + ... + prime(k).
 * https://oeis.org/A111326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that the sum of the first k primes
 *            is divisible by 12
 * Range:     u8 none, u16 n <= 3544, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: run through the primes, accumulate the sum modulo 12 and record the index when the
   running sum is 0 modulo 12.  The results are memoised in a dynamically‑grown array so that calls
   with arbitrary n are fast.  The array works correctly whether seq_t is a builtin integer type
   (C11) or an arbitrary‑precision class type (C++17). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *k_arr = NULL;        /* array of found k‑values */
static idx_t  k_cap = 0;           /* current capacity of k_arr          */
static idx_t  k_size = 0;          /* number of stored k‑values          */
static idx_t  prime_idx = 0;       /* current prime index (1‑based)      */
static unsigned int cur_mod = 0;   /* running sum modulo 12              */

/* Allocate or grow the array that holds the results.  The implementation
   works for both C (malloc/realloc) and C++ (new). */
static void grow_k_arr(void)
{
    idx_t new_cap = (k_cap == 0) ? (idx_t)16 : k_cap << 1;
    seq_t *new_arr;

#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
    /* Copy existing elements into the new block. */
    for (idx_t i = 0; i < k_size; ++i) {
        new_arr[i] = k_arr[i];
    }
    /* Destroy the old block. */
    delete [] k_arr;
#else
    new_arr = (seq_t *)realloc(k_arr, (size_t)new_cap * sizeof(seq_t));
    /* realloc automatically preserves old contents; no manual copy needed. */
#endif

    k_arr = new_arr;
    k_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* Undefined for n < 1; return 0 as a safe default. */
    }

    /* Initialise the array on the first call. */
    if (k_arr == NULL) {
        grow_k_arr();
    }

    /* Generate terms until we have at least n results. */
    while (k_size < n) {
        /* Advance to next prime. */
        ++prime_idx;
        seq_t p = get_prime(prime_idx);  /* get_prime returns seq_t */

        /* Update running sum modulo 12.  Use uint64_t to perform mod 12 safely. */
        uint64_t p64 = (uint64_t)p;      /* fits for all primes we need */
        unsigned int pm = (unsigned int)(p64 % 12);
        cur_mod = (cur_mod + pm) % 12;

        /* Record current index if the sum is 0 mod 12. */
        if (cur_mod == 0) {
            if (k_size == k_cap) {
                grow_k_arr();
            }
            k_arr[k_size++] = (seq_t)prime_idx;
        }
    }

    /* Return the (n‑th) stored k‑value (1‑based). */
    return k_arr[n - 1];
}

/*
 * A063450  Numbers k such that d(k+1) < 2*d(k), where d() is the number of
 *          divisors function A000005.
 * https://oeis.org/A063450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _divisor_count_u64_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A063450.
   a(n) = the n‑th integer k (offset 1) such that d(k+1) < 2·d(k),
   where d() is the divisor‑count function. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_divisor_count_u64_local.h"

/* Dynamic array that works both as C (malloc/realloc) and as C++ (new/delete). */
static seq_t *terms   = NULL;   /* stored sequence values                */
static size_t cap     = 0;      /* allocated capacity (in seq_t elements)*/
static idx_t  len     = 0;      /* number of terms currently stored      */
static uint64_t next_k = 2;     /* smallest k not yet tested             */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, undefined below */

    /* Ensure we have enough room in the array */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 128;
        if (new_cap < (size_t)n) new_cap = n;
#ifdef __cplusplus
        /* Allocate a new array, copy existing terms, delete the old one */
        seq_t *newptr = new seq_t[new_cap];
        for (size_t i = 0; i < cap; ++i) newptr[i] = terms[i];
        delete[] terms;
        terms = newptr;
#else
        /* C allocation with realloc */
        seq_t *newptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = newptr;
#endif
        cap = new_cap;
    }

    /* Compute terms until we’ve produced at least n of them */
    while (len < n) {
        uint64_t k   = next_k;
        uint64_t dk  = _divisor_count_u64_local(k);
        uint64_t dk1 = _divisor_count_u64_local(k + 1);
        if (dk1 < 2 * dk) {
            terms[len++] = (seq_t)k;   /* cast is safe for all seq_t types */
        }
        ++next_k;
    }

    return terms[n - 1];
}

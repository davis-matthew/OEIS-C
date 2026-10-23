/*
 * A155770  Primes of form: 2*n^2+2*n-41.
 * https://oeis.org/A155770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 82, u32 n <= 98, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes of the form 2*n^2 + 2*n - 41
   a(n) is the n‑th prime of this form (a(1) = 19), offset = 1.  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Helper routines that work for both C and C++ builds.  */
#ifdef __cplusplus
static seq_t* allocate_seq(size_t n)
{
    return new seq_t[n];
}
static seq_t* reallocate_seq(seq_t* old, size_t old_cap, size_t new_cap)
{
    seq_t* newptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        newptr[i] = old[i];
    delete[] old;
    return newptr;
}
#else
static seq_t* allocate_seq(size_t n)
{
    return (seq_t*)malloc(n * sizeof(seq_t));
}
static seq_t* reallocate_seq(seq_t* old, size_t old_cap, size_t new_cap)
{
    (void)old_cap;          /* unused in C */
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
}
#endif

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   found  = 0;      /* number of terms already stored   */
    static idx_t   next_n = 5;      /* n whose value 2*n^2+2*n-41 first
                                       might be prime (a(1)=19).         */

    /* initialise cache on the first call */
    if (cache == NULL) {
        cap = 8;
        cache = allocate_seq(cap);
        if (cache == NULL) return 0;   /* allocation failure */
    }

    /* fill cache up to at least the requested term */
    while (found < n) {
        /* compute candidate value as seq_t to avoid overflow */
        seq_t nn = (seq_t)next_n;
        seq_t val = 2 * nn * (nn + 1) - 41;

        if ((val > 1) && is_prime_big(val)) {
            if (found == cap) {
                size_t new_cap = cap << 1;
                seq_t* tmp = reallocate_seq(cache, cap, new_cap);
                if (tmp == NULL) return 0;   /* reallocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[found++] = val;
        }
        ++next_n;
    }

    return cache[n - 1];
}

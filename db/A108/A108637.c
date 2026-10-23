/*
 * A108637  Duplicate of A091296.
 * https://oeis.org/A108637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n-th smallest positive integer that is a semiprime
 *            (product of exactly two prime factors, counted with
 *            multiplicity) and whose decimal representation uses only odd
 *            digits (1,3,5,7,9)
 * Range:     i8 n <= 15, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   is_semiprime, odd_digits_only
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_semiprime.h"
#include "../utils/odd_digits_only.h"

static seq_t *cache = NULL;        /* array holding found terms             */
static size_t cache_cap = 0;       /* current sized of cache                */
static idx_t cache_count = 0;      /* number of terms stored in cache       */
static idx_t next_candidate = 9;   /* next integer to test for the sequence */

/* helper: append a new term to the cached sequence, resizing if needed */
static void cache_append(seq_t value)
{
    if (cache_count == cache_cap)
    {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr)               /* allocation failure */
            return;                 /* silently ignore beyond memory limits */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_count++] = value;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, but return 0 */

    /* generate terms until we have at least n entries */
    while (cache_count < n)
    {
        idx_t i = next_candidate++;
        if ((i & 1) == 0)              /* skip even numbers (cannot have all odd digits) */
            continue;

        if (odd_digits_only(i) && is_semiprime(i))
            cache_append((seq_t)i);
    }

    return cache[n - 1];
}

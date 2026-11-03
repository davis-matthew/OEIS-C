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
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* helper: test whether x is a semiprime (product of exactly two prime factors,
   counting multiplicity). */
static bool is_semiprime(idx_t x)
{
    idx_t temp = x;
    idx_t factor_count = 0;

    /* Since x is odd, no need to test factor 2. */
    for (idx_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0)
            {
                ++factor_count;
                if (factor_count > 2)
                    return false;      /* too many factors */
                temp /= p;
            }
        }
    }
    if (temp > 1)
        ++factor_count;                 /* remaining prime factor */

    return factor_count == 2;
}

/* helper: test if all decimal digits of x are odd */
static bool odd_digits_only(idx_t x)
{
    while (x > 0)
    {
        int d = (int)(x % 10);
        if ((d & 1) == 0)            /* even digit found */
            return false;
        x /= 10;
    }
    return true;
}

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
        seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
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

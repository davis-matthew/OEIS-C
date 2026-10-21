/*
 * A208982  Numbers n such that the next larger number with mutual Hamming
 *          distance 1 is prime.
 * https://oeis.org/A208982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A208982: Numbers n such that the next larger number with mutual Hamming
   distance 1 is prime.  This implementation scans integers and
   stores the sequence in a static cache. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Memoised cache of sequence values. */
static seq_t *cache      = NULL;      /* dynamic array of seq_t values   */
static size_t cache_cap  = 0;         /* allocated size                  */
static size_t cache_cnt  = 0;         /* number of values stored         */
static seq_t  next_scan  = (seq_t)1;  /* next integer to test for the
                                        sequence property               */

/* Ensure the cache has room for 'needed' entries. */
static void ensure_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < needed)
            new_cap = needed;
        /* C++ requires a cast from void* returned by realloc. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {            /* allocation failure – abort the program */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Test whether candidate meets the sequence condition:
   the next larger number with a single-bit difference is prime. */
static bool qualifies(seq_t x)
{
    seq_t lowZero = (seq_t)1;
    while ((x & lowZero) != (seq_t)0)    /* find lowest zero bit */
        lowZero <<= 1;
    return isPrimeGeneric(x + lowZero);
}

/* Return the n-th term of sequence A208982 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; caller should not ask for n=0 */
        return (seq_t)0;

    size_t target = (size_t)n;                 /* index is 1‑based */

    /* Generate terms until we reach the requested index. */
    while (cache_cnt < target) {
        seq_t cand = next_scan;
        while (!qualifies(cand))
            cand += (seq_t)1;                   /* advance to next integer */

        /* Store the found term. */
        ensure_cache(cache_cnt + 1);
        cache[cache_cnt++] = cand;

        /* Next scanning starts just after the last stored value. */
        next_scan = cand + (seq_t)1;
    }

    return cache[target - 1];
}

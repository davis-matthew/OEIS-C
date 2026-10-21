/*
 * A233473  Least k such that there are n triangular numbers between
 *          triangular(k) and k^2.
 * https://oeis.org/A233473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 18, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233473: a(n) = the least k such that there are n
   triangular numbers between T(k)=k(k+1)/2 and k².  The algorithm
   starts from k=1 and incrementally counts how many triangular
   numbers lie in the interval (T(k), k²).  For each new count the
   smallest k that achieves it is stored in a static cache, so that
   subsequent calls for any n are answered in O(1) (aside from the
   initial enlargements of the cache).  The implementation works
   with any unsigned integer type seq_t (including arbitrary‑precision
   classes) and with idx_t = uint64_t. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* ---------- cache for computed a(n) ----------------------------------- */
static seq_t *aVals = NULL;   /* aVals[n] = answer for n */
static size_t aSize   = 0;    /* allocated length of aVals          */
static size_t aMaxCnt = 0;    /* largest n for which a[n] is stored */
static idx_t  curK    = 1;    /* last k processed; start at 1      */

/* allocate or enlarge the cache to at least new_len elements */
static void grow_cache(size_t new_len)
{
    if (aSize >= new_len)
        return;
    size_t newSize = aSize ? aSize * 2 : 1;
    if (newSize < new_len)
        newSize = new_len;
    seq_t *tmp = (seq_t *)realloc(aVals, newSize * sizeof(seq_t));
    if (!tmp)
        abort();                /* memory exhausted */
    aVals  = tmp;
    aSize  = newSize;
}

/* initialise the cache on first invocation */
static void init_cache(void)
{
    if (aVals == NULL) {
        aVals = (seq_t *)malloc(sizeof(seq_t));
        if (!aVals)
            abort();
        aVals[0] = (seq_t)0;  /* a(0) = 0 by definition */
        aSize   = 1;
        aMaxCnt = 0;
        curK    = 1;
    }
}

/* return the sequence value for index n */
seq_t a(idx_t n)
{
    init_cache();

    /* already known? */
    if ((size_t)n <= aMaxCnt)
        return aVals[(size_t)n];

    /* extend cache until we have a[n] */
    while ((size_t)n > aMaxCnt) {
        /* test next k */
        curK++;

        /* k² and intermediate values in seq_t */
        seq_t k_seq  = (seq_t)curK;
        seq_t k_sq   = k_seq * k_seq;
        seq_t sq_arg = k_sq - (seq_t)1;
        seq_t sq     = (seq_t)1 + (seq_t)8 * sq_arg;

        /* floor sqrt(1 + 8(k²-1))   */
        seq_t s  = floor_sqrt(sq);

        /* m(k) = floor((s-1)/2)   */
        seq_t m_k = (s - (seq_t)1) / (seq_t)2;

        /* number of triangular numbers strictly between T(k) and k² */
        seq_t count;
        if (m_k > k_seq)
            count = m_k - k_seq;
        else
            count = (seq_t)0;

        /* if a new count is reached, record the smallest k for all
           missing n up to this count (handle potential jumps).      */
        if (count > (seq_t)aMaxCnt) {
            size_t new_len = (size_t)count + 1;
            grow_cache(new_len);
            for (size_t i = aMaxCnt + 1; i <= (size_t)count; ++i)
                aVals[i] = curK;
            aMaxCnt = (size_t)count;
        }
    }

    return aVals[(size_t)n];
}

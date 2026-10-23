/*
 * A008351  a(n) is the concatenation of a(n-1) and a(n-2) with a(1)=1,
 *          a(2)=2.
 * https://oeis.org/A008351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(a(n-1), a(n-2)) for n>2, a(1)=1, a(2)=2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

static seq_t pow10(idx_t exp)
{
    /* 10^exp, works for all seq_t implementations */
    return seq_pow((seq_t)10, exp);
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0; /* undefined for n<1 */

    /* Cache for already computed terms */
    static seq_t  *cache    = NULL;
    static idx_t  *len_cache = NULL;
    static size_t  size     = 0; /* largest index stored (0 if uninitialised) */
    static size_t  cap      = 0;

    /* ---- initialise cache on first call --------------------------------- */
    if (size == 0) {
        cap = 4; /* initial capacity */
        cache    = (seq_t  *)malloc(cap * sizeof(seq_t));
        len_cache = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!cache || !len_cache)
            return (seq_t)0;          /* allocation failed – abort */

        /* base cases */
        cache[1] = (seq_t)1;
        len_cache[1] = (idx_t)1;
        cache[2] = (seq_t)2;
        len_cache[2] = (idx_t)1;
        size = 2;
    }

    /* ---- handle small indices directly --------------------------------- */
    if (n <= size)
        return cache[n];

    /* ---- grow cache until it contains a(n) ----------------------------- */
    while (size < n) {
        /* the next index to compute is size+1 */
        idx_t new_len = len_cache[size] + len_cache[size-1];

        /* a(i) = a(i-1) * 10^{len(a(i-2))} + a(i-2) */
        seq_t term = cache[size] * pow10(len_cache[size-1]) + cache[size-1];

        /* enlarge storage if necessary */
        if (size + 1 >= cap) {
            cap <<= 1;
            seq_t  *tmp  = (seq_t  *)realloc(cache,    cap * sizeof(seq_t));
            idx_t  *tmp2 = (idx_t *)realloc(len_cache, cap * sizeof(idx_t));
            if (!tmp || !tmp2) {
                /* allocation failed – return zero to indicate error */
                return (seq_t)0;
            }
            cache    = tmp;
            len_cache = tmp2;
        }

        cache[size+1] = term;
        len_cache[size+1] = new_len;
        size++;
    }

    return cache[n];
}

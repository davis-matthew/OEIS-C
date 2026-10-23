/*
 * A191576  Numbers n such that number of divisors of n equals the number of
 *          1's in Gray code for n.
 * https://oeis.org/A191576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth integer for which the number of divisors of n
 * equals the number of 1-bits in the Gray code of n.
 * (i.e., τ(n) = popcount(n XOR (n>>1)) )
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"  /* for divisor_count_u64(uint64_t) */

seq_t a(idx_t n)
{
    /* Static state to allow repeated calls in arbitrary order. */
    static seq_t *seq   = NULL;   /* array of found terms */
    static size_t  cap   = 0;     /* current capacity of seq */
    static idx_t   len   = 0;     /* number of terms stored */
    static uint64_t cand = 1;     /* next candidate to test */
    static bool    init  = false; /* first-time initialization flag */

    if (!init) {
        init = true;
        seq  = NULL;
        cap  = 0;
        len  = 0;
        cand = 1;
    }

    /* Grow the result array as needed. */
    if (cap == 0) {
        cap = 4;
        seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        uint64_t d   = divisor_count_u64(cand);      /* number of divisors */
        uint64_t gray  = cand ^ (cand >> 1);         /* Gray code of cand */
        uint64_t pop   = __builtin_popcountll(gray); /* number of 1's */

        if (d == pop) {
            if (len == cap) {            /* need more space */
                cap *= 2;
                seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
            }
            seq[len++] = (seq_t)cand;    /* store the qualifying number */
        }
        cand++;                          /* move to next candidate */
    }

    return seq[n - 1]; /* zero-based array index */
}

/*
 * A077209  a(1) = 2, then the smallest squarefree number greater than the
 *          previous term that begins with the end of the previous term.
 * https://oeis.org/A077209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 2. For n > 0, a(n) is the smallest squarefree integer
 *            greater than a(n-1) whose first decimal digit equals the last
 *            decimal digit of a(n-1)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   digit_len, is_squarefree, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"   /* bool is_squarefree(seq_t)      */
#include "../utils/pow10.h"           /* seq_t pow10(idx_t)             */
#include "../utils/digit_len.h"       /* int  digit_len(seq_t)          */

/* Computes the next term given the previous one. */
static seq_t compute_next(seq_t prev)
{
    const unsigned int last_digit = (unsigned int)(prev % 10); /* 0‑9 */
    int len = digit_len(prev);                               /* >= 1 */

    /* Boundaries of the block of numbers that start with last_digit
       and have the current length. */
    seq_t blk_start = (seq_t)last_digit * pow10((idx_t)(len - 1));
    seq_t blk_end   = ((seq_t)(last_digit + 1) * pow10((idx_t)(len - 1))) - (seq_t)1;

    /* First candidate that is > prev. */
    seq_t cand = prev + (seq_t)1;
    if (cand < blk_start)
        cand = blk_start;

    while (1) {
        if (cand > blk_end) {                       /* move to next length */
            ++len;
            blk_start = (seq_t)last_digit * pow10((idx_t)(len - 1));
            blk_end   = ((seq_t)(last_digit + 1) * pow10((idx_t)(len - 1))) - (seq_t)1;
            cand = blk_start;
        }

        if (is_squarefree(cand))
            return cand;

        ++cand;                                      /* next number in the block */
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Cache to store already computed terms. */
    static seq_t *cache = NULL;
    static size_t stored = 0;   /* number of terms currently stored (max index +1) */
    static size_t cap    = 0;   /* current cache capacity in terms of elements */

    if (cap == 0) {                     /* first initialisation */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Extend cache as required */
    while (stored <= (size_t)n) {
        /* If we need an entry beyond current capacity, grow it. */
        if (stored >= cap) {
            cap = cap * 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        if (stored == 0) {
            cache[0] = (seq_t)2;                /* a(0) */
        } else {
            cache[stored] = compute_next(cache[stored - 1]); /* a(stored) */
        }
        ++stored;
    }

    return cache[(size_t)n];
}

/*
 * A176821  List of 4-tuples of twin primes q, p, p+2 and q+2 such that
 *          2*q<p<p+2<2*(q+2).
 * https://oeis.org/A176821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176821: quadruplets of twin primes (q, p, p+2, q+2)
 * satisfying 2*q < p < p+2 < 2*(q+2) .
 * The function builds the list of qualifying q’s on demand
 * and returns the requested element of the corresponding quadruplet
 * (q, p=2q+1, p+2=2q+3, q+2).  The array of q’s is cached so
 * subsequent calls work in O(1) per element after the first few.
 * The same code works for any sized seq_t; it never uses
 * floating‑point or non‑portable arithmetic. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

static seq_t *q_cache = NULL;   /* cache of qualifying q’s          */
static size_t q_cnt   = 0;      /* number of cached q’s             */
static size_t q_cap   = 0;      /* current capacity of the cache    */
static idx_t  last_checked = 1; /* last odd number examined in seq  */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */

    /* Determine which quadruplet and which element of it. */
    idx_t quad_idx = (n - 1) / 4;   /* 0‑based quadruplet index      */
    int   elem     = (int)((n - 1) % 4); /* 0:q, 1:p, 2:p+2, 3:q+2    */

    /* Grow the cache until it holds enough q’s. */
    while (q_cnt <= quad_idx) {
        /* Skip to the next odd candidate. */
        last_checked += (last_checked % 2 == 0) ? 1 : 2;

        /* Quick prime tests: q , q+2 , 2q+1 , 2q+3 all prime. */
        if (!is_prime_small((seq_t)last_checked))          continue;
        if (!is_prime_small((seq_t)(last_checked + 2)))    continue;
        if (!is_prime_small((seq_t)(last_checked * 2 + 1))) continue;
        if (!is_prime_small((seq_t)(last_checked * 2 + 3))) continue;

        /* Ensure capacity of the cache. */
        if (q_cnt >= q_cap) {
            size_t new_cap = (q_cap == 0) ? 4 : q_cap * 2;
            seq_t *tmp = (seq_t *)realloc(q_cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            q_cache = tmp;
            q_cap   = new_cap;
        }

        /* Store the qualifying q. */
        q_cache[q_cnt++] = (seq_t)last_checked;
    }

    /* Produce the requested element of the quadruplet. */
    seq_t q = q_cache[quad_idx];
    switch (elem) {
        case 0: return q;                 /* q          */
        case 1: return q * 2 + 1;         /* p  = 2q+1  */
        case 2: return q * 2 + 3;         /* p+2= 2q+3  */
        case 3: return q + 2;             /* q+2        */
        default: return 0;                /* unreachable */
    }
}

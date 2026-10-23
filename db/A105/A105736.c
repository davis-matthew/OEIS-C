/*
 * A105736  For n>2, a(n) > 0 is such that a(n-1)^2+4*a(n-2)*a(n) is a minimal
 *          square, a(1)=1,a(2)=3.
 * https://oeis.org/A105736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the smallest positive integer such that a(n-1)^2 +
 *            4*a(n-2)*a(n) is a perfect square, with a(1)=1, a(2)=3
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

/* Memoisation buffer (index 0 unused). */
static seq_t *a_mem = NULL;
static idx_t  a_cap = 0;
static idx_t  a_last = 0;

/* Return the nth term of sequence A105736. */
seq_t a(idx_t n)
{
    /* Sequence is defined for n>=1. */
    if (n == 0)
        return (seq_t)0;

    /* Initialise the memoisation buffer on first use. */
    if (a_mem == NULL) {
        a_cap = 4;                    /* indices 0..3 inclusive */
        a_mem = seq_malloc(a_cap);
        a_mem[0] = (seq_t)0;          /* unused sentinel */
        a_mem[1] = (seq_t)1;          /* a(1) */
        a_mem[2] = (seq_t)3;          /* a(2) */
        a_last  = 2;                  /* last computed index */
    }

    /* Compute any missing terms up to index n. */
    while (a_last < n) {
        /* Ensure there is space for a_last+1. */
        if (a_last + 1 >= a_cap) {
            idx_t new_cap = a_cap * 2;
            seq_t *tmp = seq_realloc(a_mem, new_cap);
            a_mem = tmp;
            a_cap = new_cap;
        }

        seq_t a_prev   = a_mem[a_last];     /* a(n-1)   */
        seq_t a_prev2  = a_mem[a_last-1];   /* a(n-2)   */

        seq_t cur_sq = a_prev * a_prev;         /* a_prev^2 */
        seq_t step   = (seq_t)4 * a_prev2;      /* 4*a_prev2 */

        /* Find the smallest k > a_prev such that (k^2 - cur_sq) is divisible
           by step; that gives the minimal square. */
        seq_t k = a_prev + (seq_t)1;
        for (;;) {
            seq_t k_sq = k * k;
            seq_t diff = k_sq - cur_sq;   /* diff > 0 */
            if ((diff % step) == (seq_t)0) {
                a_mem[a_last+1] = diff / step;
                break;
            }
            ++k;
        }

        /* Advance to the next index. */
        ++a_last;
    }

    return a_mem[n];
}

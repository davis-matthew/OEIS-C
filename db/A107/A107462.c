/*
 * A107462  Numbers of the form (4^i)*(13^j), with i, j >= 0.
 * https://oeis.org/A107462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   push_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107462: numbers of the form 4^i * 13^j (i, j ≥ 0).  
   Generated in increasing order by a two‑pointer merge algorithm. */

#include <stdlib.h>
#include "../utils/push_value.h"

/* Global state used by the two‑pointer algorithm. */
static seq_t *g_seq = NULL;      /* array holding the sequence */
static size_t  g_cap = 0;        /* current capacity of g_seq */
static idx_t   g_size = 0;       /* number of terms stored */
static idx_t   g_ptr4 = 0;       /* next index for 4× factor */
static idx_t   g_ptr13 = 0;      /* next index for 13× factor */

/* Required entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n < 1, but harmless */

    while (g_size < n) {
        seq_t next;

        if (g_size == 0) {            /* initialise with 1 */
            next = (seq_t)1;
        } else {                      /* merge candidates from both pointers */
            seq_t cand4  = g_seq[g_ptr4]  * (seq_t)4;
            seq_t cand13 = g_seq[g_ptr13] * (seq_t)13;
            next = (cand4 < cand13) ? cand4 : cand13;
        }

        push_value(&g_seq, &g_cap, &g_size, next);

        if (g_size > 1) {              /* update pointers that contributed */
            seq_t cand4  = g_seq[g_ptr4]  * (seq_t)4;
            seq_t cand13 = g_seq[g_ptr13] * (seq_t)13;
            if (cand4  == next) g_ptr4++;
            if (cand13 == next) g_ptr13++;
        }
    }

    return g_seq[n - 1];
}

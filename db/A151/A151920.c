/*
 * A151920  a(n) = (Sum_{i=1..n+1} 3^wt(i))/3, where wt() = A000120().
 * https://oeis.org/A151920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = (Σ_{i=1..n+1} 3^popcount(i))/3 (popcount is binary
 *            weight)
 * Range:     u8 n <= 14, u16 n <= 492, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Equivalent recurrence: a(n) = a(n-1) + 3^popcount(n+1)/3,  a(0)=1 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow3.h"

/* Compute binary population count of a 64‑bit integer (uses compiler builtin). */
static inline int popcount_u64(idx_t v) {
    return (int)__builtin_popcountll(v);
}

extern seq_t a(idx_t n) {
    /* P[k] = Σ_{i=1..k} 3^popcount(i)  (k ≥ 0) */
    static seq_t *P = NULL;   /* dynamic array of cumulative sums */
    static idx_t cap = 0;     /* number of entries currently stored */

    /* Need indices 0 .. n+1 inclusive (total n+2 entries) */
    idx_t need = n + 2;
    if (cap < need) {
        seq_t *newP = (seq_t *)realloc(P, need * sizeof(seq_t));
        if (!newP) {
            /* Allocation failed – abort (as in many OEIS test harnesses) */
            exit(1);
        }
        P = newP;

        /* Initialize P[0] on first allocation */
        if (cap == 0) {
            P[0] = 0;
            cap = 1;  /* now one element exists */
        }

        /* Compute new cumulative sums */
        for (idx_t k = cap; k < need; ++k) {
            int pc = popcount_u64(k);          /* popcount of k (k ≥ 0) */
            seq_t term = pow3((idx_t)pc);      /* 3^pc */
            P[k] = P[k - 1] + term;            /* accumulate */
        }
        cap = need;
    }

    /* a(n) = P[n+1] / 3 , exact division */
    return P[n + 1] / 3;
}

/*
 * A170416  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n≤43: a(n)=2+21·∑_{k=0}^{n-1} a(k) for n=44:
 *            a(44)=1+21·∑_{k=1}^{43} a(k) – 231·a(0) for n>44:
 *            a(n)=21·∑_{k=n-43}^{n-1} a(k) – 231·a(n-44)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */
#include <stdbool.h>     /* for bool */
#include <stdlib.h>      /* for malloc, realloc, free */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*, size_t old, size_t new) */

seq_t a(idx_t n)
{
    static seq_t *a_arr   = NULL;   /* array of sequence terms          */
    static seq_t *cum    = NULL;   /* array of cumulative sums         */
    static size_t cap     = 0;      /* allocated capacity (in elements) */
    static idx_t  computed = 0;     /* largest index that has been filled */
    static bool initialized = false;

    /* ------------------------------------------------------------------ */
    /* Allocate or grow the arrays so that they contain indices 0..n       */
    /* ------------------------------------------------------------------ */
    if (!initialized || n >= (idx_t)cap) {
        size_t new_cap = cap ? (cap << 1) : 64;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        a_arr = (seq_t *)seq_realloc(a_arr, cap, new_cap);
        cum  = (seq_t *)seq_realloc(cum,  cap, new_cap);
        cap  = new_cap;
    }

    /* ------------------------------------------------------------------ */
    /* Perform one‑time initialization: a(0)=1, cum(0)=1                  */
    /* ------------------------------------------------------------------ */
    if (!initialized) {
        a_arr[0] = (seq_t)1;
        cum[0]   = (seq_t)1;
        computed = 0;
        initialized = true;
    }

    /* ------------------------------------------------------------------ */
    /* Compute all missing terms up to index n                              */
    /* ------------------------------------------------------------------ */
    while (computed < n) {
        size_t i = computed + 1;               /* next index to fill   */
        seq_t val;

        if (i <= 43) {                          /* n ≤ 43               */
            val = (seq_t)2 + ((seq_t)21 * cum[i - 1]);
        } else if (i == 44) {                   /* n = 44               */
            seq_t sum43 = cum[i - 1] - cum[i - 44];   /* a1 … a43            */
            val = (seq_t)1 + ((seq_t)21 * sum43) - ((seq_t)231 * a_arr[0]);
        } else {                                /* n > 44               */
            seq_t sum43 = cum[i - 1] - cum[i - 44];   /* last 43 terms       */
            val = ((seq_t)21 * sum43) - ((seq_t)231 * a_arr[i - 44]);
        }

        a_arr[i] = val;
        cum[i]   = cum[i - 1] + val;
        computed = i;
    }

    return a_arr[n];
}

/*
 * A056010  Number of words of length n in a simple grammar.
 * https://oeis.org/A056010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   prod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056010: Number of words of length n in a simple grammar
   Recurrence: a(0)=1, a(1)=1,
                a(n) = 2*a(n-1) + Σ_{i=0}^{n-2} a(i)*a(n-2-i)  (n>1) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/prod_mul.h"

static seq_t *A = NULL;    /* dynamic array for values a(0)..a(k) */
static size_t A_cap = 0;   /* current allocated capacity */

/* Grow the array to contain at least min_cap elements, zero‑initialising new slots. */
static void ensure_cap(size_t min_cap)
{
    if (A_cap >= min_cap) return;
    size_t new_cap = (A_cap > 0) ? A_cap : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) return;            /* allocation failure will be caught later */
    for (size_t i = A_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    A = tmp;
    A_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure capacity for n+1 elements. */
    ensure_cap((size_t)n + 1);

    /* Initialise base cases once. */
    if (A_cap <= 0) {                  /* very first call */
        A[0] = (seq_t)1;
        if (A_cap > 1) A[1] = (seq_t)1;
    } else { /* when capacity grows later */
        if (A[0] == (seq_t)0) A[0] = (seq_t)1;
        if (A_cap > 1 && A[1] == (seq_t)0) A[1] = (seq_t)1;
    }

    if ((size_t)n < 2) return A[(size_t)n];

    /* Compute missing values up to n. */
    for (size_t k = (size_t)2; k <= (size_t)n; ++k) {
        if (A[k] != (seq_t)0) continue;   /* already computed */

        seq_t sum = (seq_t)0;
        for (size_t i = 0; i + 1 < k; ++i) {
            sum += prod_mul(A[i], A[k - 2 - i]);
        }
        A[k] = A[k - 1] + A[k - 1] + sum;  /* 2*a(k-1) + sum */
    }

    return A[(size_t)n];
}

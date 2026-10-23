/*
 * A169271  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence for A169271 of order 29:
   a(0) = 1.
   For n ≥ 1 let
        b_n = 1  if n == 0 or n == 29,
             2  if 1 ≤ n ≤ 28,
             0  otherwise.
   Then
        a(n) = 16 · Σ_{k=1}^{28} a(n−k) – 136 · a(n−29) + b_n.
   (For n ≤ 28 this reduces to a(n) = 18·17ⁿ⁻¹.) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* memoisation array */
    static size_t cap = 0;             /* current capacity */
    static idx_t next_needed = 1;      /* index of first not yet computed */

    /* Allocate or enlarge cache to hold indices 0 … n */
    if (!cache) {
        cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        next_needed = 1;
    } else if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    while (next_needed <= n) {
        idx_t i = next_needed;
        seq_t sum = 0;

        /* Σ_{k=1}^{28} a(i−k)  (ignore negative indices) */
        for (idx_t k = 1; k <= 28 && i >= k; ++k) {
            sum = sum + cache[i - k];
        }

        /* a(i−29) term if it exists */
        seq_t term29 = 0;
        if (i >= 29) {
            term29 = cache[i - 29];
        }

        /* b_i from the numerator */
        seq_t bi;
        if (i == 0 || i == 29) {
            bi = (seq_t)1;
        } else if (i <= 28) {
            bi = (seq_t)2;
        } else {
            bi = (seq_t)0;
        }

        cache[i] = (seq_t)16 * sum - (seq_t)136 * term29 + bi;
        ++next_needed;
    }

    return cache[n];
}

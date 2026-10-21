/*
 * A065355  a(n) = n! - Sum_{k=0..n-1} k!.
 * https://oeis.org/A065355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! - Sum_{k=0}^{n-1} k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 
   Compute a(n) for OEIS sequence A065355.
   Uses a memoization table to store factorials and their cumulative sums.
   Works for any seq_t that supports +, -, * with idx_t values.
*/
seq_t a(idx_t n) {
    /* Static tables for factorials and cumulative sums */
    static seq_t *fact = NULL;
    static seq_t *sum_fact = NULL;
    static idx_t size = 0;       /* number of entries stored (fact[0..size-1]) */
    static idx_t capacity = 0;   /* allocated capacity */

    /* Helper to grow tables up to at least "need" entries */
    if (size == 0) {
        /* initial allocation */
        capacity = 16;
        fact = (seq_t *)malloc(capacity * sizeof(seq_t));
        sum_fact = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!fact || !sum_fact) return 0; /* out of memory, return 0 as safe fallback */
        fact[0] = 1;           /* 0! = 1 */
        sum_fact[0] = 1;       /* sum of 0! */
        size = 1;
    }
    if (n >= size) {
        idx_t need = n + 1;   /* we need entries 0..n inclusive */
        if (need > capacity) {
            /* Expand capacity exponentially to avoid frequent reallocs */
            while (capacity < need) capacity <<= 1;
            fact = (seq_t *)realloc(fact, capacity * sizeof(seq_t));
            sum_fact = (seq_t *)realloc(sum_fact, capacity * sizeof(seq_t));
            if (!fact || !sum_fact) return 0; /* out of memory */
        }
        /* Compute factorials and cumulative sums up to n */
        for (idx_t i = size; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
            sum_fact[i] = sum_fact[i - 1] + fact[i];
        }
        size = need;
    }

    /* Compute the sequence value */
    if (n == 0) return 1;          /* a(0) = 1 by definition */
    return fact[n] - sum_fact[n - 1];
}

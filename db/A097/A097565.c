/*
 * A097565  a(0) = 0, a(1) = 1, a(n) = (a(n-1) mod F(n))*a(n-1) + a(n-2) for n
 *          > 2 where F(n) is the n-th Fibonacci number.
 * https://oeis.org/A097565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 12, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097565
   Formula: a(0) = 0, a(1) = 1, a(n) = (a(n-1) mod F(n))*a(n-1) + a(n-2)  (n>1)
   where F(n) is the n-th Fibonacci number (F(0)=0, F(1)=1). */
#include <stdlib.h>
#include <stdint.h>

/* Helper to grow a dynamic array of seq_t */
static void grow_seq(void **array, idx_t *cap, idx_t need)
{
    if (*cap >= need) return;
    idx_t new_cap = (*cap == 0) ? 4 : *cap;
    while (new_cap < need) new_cap <<= 1;
    *array = realloc(*array, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* The sequence function */
seq_t a(idx_t n)
{
    /* Static storage for the sequence and Fibonacci values. */
    static seq_t *a_vals = NULL, *f_vals = NULL;
    static idx_t a_cap = 0, f_cap = 0;
    static idx_t a_len = 0, f_len = 0;

    /* Initialisation: create the first two terms of each array. */
    if (a_len == 0) {
        /* a(0) = 0, a(1) = 1 */
        grow_seq((void**)&a_vals, &a_cap, 2);
        a_vals[0] = (seq_t)0;
        a_vals[1] = (seq_t)1;
        a_len = 2;

        /* Fibonacci numbers: F(0)=0, F(1)=1 */
        grow_seq((void**)&f_vals, &f_cap, 2);
        f_vals[0] = (seq_t)0;
        f_vals[1] = (seq_t)1;
        f_len = 2;
    }

    /* Extend Fibonacci table to index n (needed for mod operation). */
    if (n + 1 > f_len) {
        grow_seq((void**)&f_vals, &f_cap, n + 1);
        for (idx_t i = f_len; i <= n; ++i)
            f_vals[i] = f_vals[i-1] + f_vals[i-2];
        f_len = n + 1;
    }

    /* Extend a(n) table up to n, computing new terms using the recurrence. */
    if (n + 1 > a_len) {
        grow_seq((void**)&a_vals, &a_cap, n + 1);
        for (idx_t i = a_len; i <= n; ++i) {
            seq_t prev1 = a_vals[i-1];
            seq_t prev2 = a_vals[i-2];
            seq_t mod   = prev1 % f_vals[i];
            a_vals[i] = mod * prev1 + prev2;
        }
        a_len = n + 1;
    }

    return a_vals[n];
}

/*
 * A062724  a(n) = floor(phi^n) + 1, where phi = (1 + sqrt(5))/2.
 * https://oeis.org/A062724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*Fib(n-1) + Fib(n-2) + (n mod 2) for n > 0, with a(0)=2,
 *            a(1)=2 (Fib(0)=0, Fib(1)=1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(idx_t n) as described above using dynamic programming.
   The function is thread‑local thanks to static data; OK for single‑thread usage.
   It works for any seq_t with +, *, +, and assignment. */
#include <stdlib.h>

static seq_t *fib_arr = NULL;   /* Fibonacci numbers */
static seq_t *a_arr    = NULL;   /* Sequence a(n) */
static size_t    max_n   = 1; /* largest n computed so far (at least 1) */
static size_t    cap     = 0;   /* current allocated capacity */

/* Ensure the arrays can hold indices up to idx (inclusive). */
static void ensure_capacity(size_t idx)
{
    if (idx < cap) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap <= idx) new_cap <<= 1;
    fib_arr = (seq_t *)realloc(fib_arr, new_cap * sizeof(seq_t));
    a_arr  = (seq_t *)realloc(a_arr,  new_cap * sizeof(seq_t));
    cap = new_cap;
    /* No need to initialise new elements; we will assign them before use. */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Initialise the arrays if this is the first call. */
    if (fib_arr == NULL) {
        ensure_capacity(1);
        fib_arr[0] = (seq_t)0;
        fib_arr[1] = (seq_t)1;
        a_arr[0]  = (seq_t)2;
        a_arr[1]  = (seq_t)2;
        max_n = 1;
    }

    /* Grow arrays up to the requested index. */
    if (idx > max_n) {
        ensure_capacity(idx);
        for (size_t i = max_n + 1; i <= idx; ++i) {
            /* Compute Fibonacci[i] = Fibonacci[i-1] + Fibonacci[i-2] */
            fib_arr[i] = fib_arr[i-1] + fib_arr[i-2];
            /* Compute a(i) using the closed recurrence for n > 1. */
            seq_t term = (seq_t)3 * fib_arr[i-1] + fib_arr[i-2];
            if (i & 1) term = term + (seq_t)1; /* add (n mod 2) */
            a_arr[i] = term;
        }
        max_n = idx;
    }

    return a_arr[idx];
}

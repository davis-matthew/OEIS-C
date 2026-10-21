/*
 * A090208  Sums of two consecutive nonprime Fibonacci numbers (A090206).
 * https://oeis.org/A090208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = A090206(n) + A090206(n+1)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 20, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A090208,
   the sum of two consecutive non‑prime Fibonacci numbers. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Maximum n that callers may request (allow cushion) */
#define MAX_N_FOR_STORAGE 10020

/* Static cache of non‑prime Fibonacci numbers */
static seq_t np[MAX_N_FOR_STORAGE];
static size_t np_cnt = 0;          /* number of stored nonprime fibs    */
static seq_t fib_prev = 0;        /* F(i‑1) of the next Fibonacci number to generate */
static seq_t fib_cur  = 1;        /* F(i)   of the next Fibonacci number to generate   */
static bool initialized = false;

/* Push a value onto the nonprime array. Assumes sufficient capacity. */
static void np_push(seq_t val) {
    np[np_cnt++] = val;
}

/* Initialise static state on first call */
static void init_state(void) {
    if (initialized) return;
    np_push((seq_t)0);           /* A090206(1) */
    np_push((seq_t)1);           /* A090206(2) */
    fib_prev = (seq_t)0;
    fib_cur  = (seq_t)1;
    initialized = true;
}

/* Generate nonprime Fibonacci numbers until we have at least `target` entries.
   `target` is the number of non‑prime Fibonacci numbers required (i.e. n+1). */
static void ensure_nonprime(idx_t target) {
    while (np_cnt < (size_t)target) {
        seq_t next = fib_prev + fib_cur;  /* next Fibonacci number */
        fib_prev = fib_cur;
        fib_cur  = next;

        /* 0 and 1 are treated as non‑prime by the sequence definition */
        if (!is_prime_big(next))
            np_push(next);
    }
}

/* Public sequence function */
seq_t a(idx_t n) {
    /* a(1) is defined; n >= 1 by problem statement */
    init_state();
    ensure_nonprime(n + 1);                          /* need A090206(n) and A090206(n+1) */
    return np[(size_t)(n - 1)] + np[(size_t)(n)];    /* 1‑based → 0‑based index */
}

/*
 * A177195  Fibonacci numbers whose decimal expansion does not contain the
 *          digit 1.
 * https://oeis.org/A177195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 22, u64 n <= 26, u128 n <= 28,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A177195: Fibonacci numbers whose decimal expansion does not contain the digit 1.
   Algorithm: iterate over Fibonacci numbers, test each for absence of digit '1' using digit_counts, 
   and store qualifying terms in a growing array. The function returns the n-th qualifying term
   (1‑based index). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n) {
    /* static storage for the sequence and the Fibonacci generator */
    static seq_t *seq = NULL;        /* array of qualifying Fibonacci numbers */
    static size_t seq_cap = 0;       /* allocated capacity */
    static size_t seq_len = 0;       /* number of stored terms */
    static seq_t fib_prev = 0;       /* F(k)   */
    static seq_t fib_curr = 1;       /* F(k+1) */
    static bool initialized = false;

    if (!initialized) {
        /* initialise storage */
        seq_cap = 32;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        /* Process F(0)=0 initially */
        initialized = true;
    }

    /* local buffer for digit counts */
    unsigned char cnt[10];

    /* generate terms until we have at least n terms stored */
    while (seq_len < n) {
        /* test current Fibonacci number */
        digit_counts(fib_prev, cnt);
        if (cnt[1] == 0) {
            /* store term, resizing if needed */
            if (seq_len + 1 > seq_cap) {
                seq_cap = seq_cap * 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = fib_prev;
        }
        /* advance Fibonacci sequence: F(k+2) = F(k) + F(k+1) */
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }

    return seq[n - 1];
}

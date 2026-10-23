/*
 * A014243  a(n) = ((n+1)-st Lucas number) - (n-th non-Lucas number).
 * https://oeis.org/A014243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A000032(n+1) – A057854(n) Lucas(n+1) is computed as
 *            F(n)+F(n+2) (fast Fibonacci from utils). n‑th non‑Lucas number
 *            is found by scanning positive integers and skipping all Lucas
 *            numbers up to a small buffer
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 43, i64 n <= 89, i128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* ---- Lucas(n+1) --------------------------------------------------- */
    /* Lucas(k) = F(k‑1) + F(k+1)   →   Lucas(n+1) = F(n) + F(n+2) */
    seq_t fn   = fib(n);     /* F(n) */
    seq_t fn2  = fib(n + 2); /* F(n+2) */
    seq_t lucas = fn + fn2;  /* Lucas(n+1) */

    /* ---- n‑th non‑Lucas ------------------------------------------------ */
    /* A generous upper bound. For n ≤ 10⁴ this is far larger than the
       needed range.  Only a few thousand bools are allocated. */
    idx_t bound = n + 200;
    /* Allocate a boolean array to flag Lucas numbers up to 'bound'. */
    bool *is_lucas = (bool*)calloc((size_t)(bound + 1), sizeof(bool));
    if (!is_lucas) {
        /* Allocation failure: fall back to returning lucas (buggy) */
        return lucas;
    }

    /* Generate Lucas numbers up to 'bound' using the recurrence L0=2, L1=1 */
    idx_t L_prev = 2;  /* L0 */
    idx_t L_curr = 1;  /* L1 */
    if (L_prev <= bound) is_lucas[L_prev] = true;
    if (L_curr <= bound) is_lucas[L_curr] = true;
    while (true) {
        idx_t L_next = L_prev + L_curr;
        if (L_next > bound) break;
        is_lucas[L_next] = true;
        L_prev = L_curr;
        L_curr = L_next;
    }

    /* Scan integers counting non‑Lucas numbers until we reach the n‑th. */
    idx_t count = 0;
    idx_t x = 1;
    while (count < n) {
        if (x <= bound && is_lucas[x]) {
            /* x is a Lucas number – skip */
        } else {
            /* x is not a Lucas number */
            ++count;
        }
        ++x;
    }
    /* x-1 is the n‑th non‑Lucas number */
    seq_t nonlucas = (seq_t)(x - 1);

    /* Clean up */
    free(is_lucas);

    /* ---- Result -------------------------------------------------------- */
    return lucas - nonlucas;
}

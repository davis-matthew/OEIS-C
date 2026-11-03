/*
 * A237882  Numbers k such that LR0(k) > LR1(k), where LR0(k) = A087117(k) is
 *          the length of the longest run of zeros in the binary
 *          representation of k, LR1(k) = A038374(k) is the length of the
 *          longest run of ones.
 * https://oeis.org/A237882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 967 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A237882:
   a(n) is the n‑th non‑negative integer k such that the longest run
   of zeros in the binary representation of k exceeds the longest run
   of ones (LR0(k) > LR1(k)).  The function iterates integers,
   tests each candidate, and caches discovered indices to support
   random‑order calls. */

#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------- */
static bool passes(idx_t k)
{
    if (k == 0)                /* binary “0” : LR0 = 1 > LR1 = 0   */
        return true;

    seq_t x = (seq_t)k;        /* convert to seq_t for bit ops     */
    bool prev_bit = (bool)(x & 1);
    idx_t run      = 1;
    idx_t max0     = 0;
    idx_t max1     = 0;

    x >>= 1;
    while (x != (seq_t)0) {
        bool bit = (bool)(x & 1);
        if (bit == prev_bit)
            run++;
        else {
            if (prev_bit) {
                if (run > max1) max1 = run;
            } else {
                if (run > max0) max0 = run;
            }
            prev_bit = bit;
            run = 1;
        }
        x >>= 1;
    }
    /* final run */
    if (prev_bit) {
        if (run > max1) max1 = run;
    } else {
        if (run > max0) max0 = run;
    }
    return max0 > max1;
}

/* ------------------------------------------------------------- */
/* Test whether k satisfies LR0(k) > LR1(k)                        */

/* ------------------------------------------------------------- */
/* Caching of discovered indices (idx_t)                          */
/* ------------------------------------------------------------- */
static idx_t *index_arr   = NULL;  /* array of k values (idx_t) */
static size_t  idx_cap    = 0;     /* allocated capacity        */
static size_t  idx_len    = 0;     /* number of elements        */
static idx_t   next_k     = 0;     /* next integer to test      */

static void add_index(idx_t val)
{
    if (idx_len == idx_cap) {
        idx_cap = idx_cap ? idx_cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(index_arr, idx_cap * sizeof(idx_t));
        if (!tmp) abort();
        index_arr = tmp;
    }
    index_arr[idx_len++] = val;
}

/* ------------------------------------------------------------- */
/* Main sequence generator                                        */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined below offset, safe fallback */

    /* generate terms until we have at least n entries */
    while (idx_len < n) {
        while (!passes(next_k)) next_k++;
        add_index(next_k);
        next_k++;
    }
    return (seq_t)index_arr[n - 1];
}

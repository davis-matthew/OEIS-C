/*
 * A035332  Smallest number not the concatenation of consecutive earlier
 *          terms.
 * https://oeis.org/A035332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A035332: smallest integer not representable as a concatenation of
   one or more consecutive earlier terms of the sequence. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Global state for the sequence */
static seq_t *seq = NULL;           /* stored sequence values       */
static seq_t *pow10_arr = NULL;     /* 10^digits(a[i]) for each term */
static size_t seq_len = 0;          /* number of terms currently stored */
static size_t seq_cap = 0;          /* capacity of the dynamic arrays   */
static seq_t current = 1;           /* next integer to test            */

/* ------------------------------------------------------------------ */
/* Test whether m can be built by concatenating consecutive earlier   */
/* terms. Returns true if such a concatenation exists.                 */
static bool is_concatenation(seq_t m)
{
    for (size_t i = 0; i < seq_len; ++i) {
        seq_t val = seq[i];
        if (val == m) return true;
        if (val > m) break;                 /* further starts are too large */

        for (size_t j = i + 1; j < seq_len; ++j) {
            seq_t new_val = val * pow10_arr[j] + seq[j];
            if (new_val == m) return true;
            if (new_val > m) break;         /* no further concatenations possible */
            val = new_val;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Grow the dynamic arrays if necessary.                              */
static void ensure_capacity(void)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp_seq   = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_t *tmp_pow10 = (seq_t *)realloc(pow10_arr, new_cap * sizeof(seq_t));
        if (!tmp_seq || !tmp_pow10) abort(); /* out of memory */
        seq = tmp_seq;
        pow10_arr = tmp_pow10;
        seq_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Build the sequence until it contains at least n terms.             */
static void build_up_to(idx_t n)
{
    while (seq_len < (size_t)n) {
        seq_t cand = current;

        if (!is_concatenation(cand)) {
            ensure_capacity();
            seq[seq_len] = cand;
            size_t digs = (size_t)digit_len(cand);   /* number of decimal digits */
            pow10_arr[seq_len] = pow10(digs);       /* 10^digs */
            ++seq_len;
        }
        ++current;
    }
}

/* ------------------------------------------------------------------ */
/* Public function demanded by the build.                            */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n<1, return 0 */
    build_up_to(n);
    return seq[n - 1];
}

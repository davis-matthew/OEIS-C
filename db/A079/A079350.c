/*
 * A079350  "Look and Say" (or LS) transform of A079342(n). These terms are
 *          such that A079342(n) divides a(n).
 * https://oeis.org/A079350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = LS(A079342(n))
   Computed by scanning integers in increasing order, selecting those
   n that divide LS(n).  For each such n the value LS(n) is stored
   as a(n).  Resulting array of a(n) values is memoised for fast
   repeated calls. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* helper: compute LS of candidate and return (valid, LS_value).
   candidate is idx_t; LS_value is seq_t.  Returns true if
   LS(candidate) % candidate == 0. */
static bool compute_ls_and_check(idx_t candidate, seq_t *out_ls)
{
    /* digits of candidate in most significant first order */
    uint8_t digits[64];
    size_t len = 0;
    idx_t tmp = candidate;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        uint8_t rev[64];
        size_t rev_len = 0;
        while (tmp > 0) {
            rev[rev_len++] = tmp % 10;
            tmp /= 10;
        }
        for (size_t i = 0; i < rev_len; ++i)
            digits[i] = rev[rev_len - 1 - i];
        len = rev_len;
    }

    seq_t ls_val = 0;
    seq_t mod_val = 0;
    seq_t cand_seq = (seq_t)candidate;

    uint8_t cur = digits[0];
    size_t run = 1;
    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == cur) {
            ++run;
        } else {
            /* flush current run */
            /* append run length digits */
            uint8_t run_buf[32];
            size_t run_len = 0;
            size_t rtmp = run;
            while (rtmp > 0) {
                run_buf[run_len++] = rtmp % 10;
                rtmp /= 10;
            }
            for (size_t k = run_len; k-- > 0;) {
                uint8_t dgt = run_buf[k];
                ls_val = ls_val * 10 + dgt;
                mod_val = (mod_val * 10 + dgt) % cand_seq;
            }
            /* append the digit itself */
            ls_val = ls_val * 10 + cur;
            mod_val = (mod_val * 10 + cur) % cand_seq;

            /* start new run */
            cur = digits[i];
            run = 1;
        }
    }
    /* flush last run */
    uint8_t run_buf[32];
    size_t run_len = 0;
    size_t rtmp = run;
    while (rtmp > 0) {
        run_buf[run_len++] = rtmp % 10;
        rtmp /= 10;
    }
    for (size_t k = run_len; k-- > 0;) {
        uint8_t dgt = run_buf[k];
        ls_val = ls_val * 10 + dgt;
        mod_val = (mod_val * 10 + dgt) % cand_seq;
    }
    ls_val = ls_val * 10 + cur;
    mod_val = (mod_val * 10 + cur) % cand_seq;

    if (mod_val == 0) {
        *out_ls = ls_val;
        return true;
    }
    return false;
}

/* memoised storage for computed a(n) values */
static seq_t *a_vals = NULL;
static size_t a_cap = 0;
static size_t a_len = 0;
static idx_t last_candidate = 0;

/* ensure capacity for a_vals at least need */
static void ensure_capacity(size_t need)
{
    if (need <= a_cap)
        return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_arr = mem_new(seq_t, new_cap);
    if (!new_arr) {
        /* unable to allocate, terminate */
        exit(1);
    }
    if (a_vals) {
        mem_copy(new_arr, a_vals, a_len * sizeof(seq_t));
        mem_free(a_vals);
    }
    a_vals = new_arr;
    a_cap = new_cap;
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    while (a_len < n) {
        ++last_candidate;
        seq_t ls_val;
        if (compute_ls_and_check(last_candidate, &ls_val)) {
            ensure_capacity(a_len + 1);
            a_vals[a_len++] = ls_val;
        }
    }
    return a_vals[n - 1];
}

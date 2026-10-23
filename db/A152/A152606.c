/*
 * A152606  a(1) = 1; thereafter a(n) is always the smallest integer > a(n-1)
 *          not leading to a contradiction, such that any six consecutive
 *          digits in the sequence sum up to a prime.
 * https://oeis.org/A152606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152606: smallest integer > previous such that any six
   consecutive decimal digits in the concatenation of the terms sum to a
   prime.  The sequence is built incrementally and memoised.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

/* ---------- Prime table for sums 0‑60 --------------------------------- */
static bool prime_tbl[61];
static bool prime_tbl_init = false;
static void init_prime_tbl(void)
{
    if (prime_tbl_init) return;
    for (int i = 0; i <= 60; ++i) prime_tbl[i] = true;
    prime_tbl[0] = prime_tbl[1] = false;
    for (int i = 2; i * i <= 60; ++i)
        if (prime_tbl[i])
            for (int j = i * i; j <= 60; j += i)
                prime_tbl[j] = false;
    prime_tbl_init = true;
}

/* ---------- Global state --------------------------------------------- */
struct SeqState {
    seq_t*  terms;          /* memoised terms */
    size_t size;            /* number of terms stored */
    size_t capacity;        /* malloced array capacity */
    uint8_t last5[5];       /* last up to five decimal digits */
    int   cnt5;             /* number of digits stored in last5 (≤5) */
    int   sum5;             /* sum of digits in last5 */
    size_t total_digits;    /* total digits generated so far */
} seq_state = { NULL, 0, 0, {0}, 0, 0, 0 };

/* Ensure at least 'needed' capacity in terms array.  */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_state.capacity) return;
    size_t new_cap = seq_state.capacity ? seq_state.capacity << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t* new_terms = alloc_seq_array(new_cap);
    if (seq_state.terms) {
        for (size_t i = 0; i < seq_state.size; ++i)
            new_terms[i] = seq_state.terms[i];
    }
    seq_state.terms   = new_terms;
    seq_state.capacity = new_cap;
}

/* ---------- Digit handling ------------------------------------------- */
static bool candidate_ok(seq_t cand)
{
    /* copy current window state */
    uint8_t  tmp_last5[5];
    int      tmp_cnt5 = seq_state.cnt5;
    int      tmp_sum5 = seq_state.sum5;
    size_t   tmp_tot  = seq_state.total_digits;
    for (int i = 0; i < 5; ++i) tmp_last5[i] = seq_state.last5[i];

    /* digits of cand, collected reverse */
    seq_t zero = 0, ten = 10;
    seq_t tmp  = cand;
    size_t rev_len = 0;
    uint8_t rev[1024];
    if (tmp == zero) {
        rev[rev_len++] = 0;
    } else {
        while (tmp > zero) {
            rev[rev_len++] = (uint8_t)(tmp % ten);
            tmp /= ten;
        }
    }

    /* process digits in natural order */
    for (size_t i = 0; i < rev_len; ++i) {
        uint8_t d = rev[rev_len - 1 - i];
        /* test new 6‑digit windows */
        if (tmp_tot >= 5) {
            int sum6 = tmp_sum5 + d;
            if (!prime_tbl[sum6]) return false;
        }
        /* update sliding window */
        if (tmp_cnt5 < 5) {
            tmp_last5[tmp_cnt5++] = d;
            tmp_sum5 += d;
        } else {
            uint8_t old = tmp_last5[0];
            for (int j = 0; j < 4; ++j) tmp_last5[j] = tmp_last5[j + 1];
            tmp_last5[4] = d;
            tmp_sum5 = tmp_sum5 - old + d;
        }
        ++tmp_tot;
    }
    return true;
}

/* Commit the chosen term: update the digit window state. */
static void commit_candidate(seq_t val)
{
    seq_t zero = 0, ten = 10;
    seq_t tmp  = val;
    size_t rev_len = 0;
    uint8_t rev[1024];
    if (tmp == zero) {
        rev[rev_len++] = 0;
    } else {
        while (tmp > zero) {
            rev[rev_len++] = (uint8_t)(tmp % ten);
            tmp /= ten;
        }
    }

    for (size_t i = 0; i < rev_len; ++i) {
        uint8_t d = rev[rev_len - 1 - i];
        if (seq_state.total_digits >= 5) {
            /* the 6‑digit window has already passed the prime test.  */
        }
        if (seq_state.cnt5 < 5) {
            seq_state.last5[seq_state.cnt5++] = d;
            seq_state.sum5 += d;
        } else {
            uint8_t old = seq_state.last5[0];
            for (int j = 0; j < 4; ++j) seq_state.last5[j] = seq_state.last5[j + 1];
            seq_state.last5[4] = d;
            seq_state.sum5 = seq_state.sum5 - old + d;
        }
        ++seq_state.total_digits;
    }
}

/* ---------- Public function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* not defined for n=0 */
    init_prime_tbl();

    /* initialise on first call */
    if (seq_state.size == 0) {
        ensure_capacity(16);
        seq_state.terms[0] = (seq_t)1;
        seq_state.size = 1;
        seq_state.total_digits = 1;
        seq_state.cnt5 = 1;
        seq_state.sum5 = 1;
        seq_state.last5[0] = 1;
    }

    /* generate terms up to n */
    while (seq_state.size < n) {
        seq_t cand = seq_state.terms[seq_state.size - 1] + (seq_t)1;
        while (!candidate_ok(cand))
            cand = cand + (seq_t)1;
        ensure_capacity(seq_state.size + 1);
        seq_state.terms[seq_state.size++] = cand;
        commit_candidate(cand);
    }

    return seq_state.terms[n - 1];
}

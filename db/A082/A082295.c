/*
 * A082295  Numbers having more than two square divisors > 1.
 * https://oeis.org/A082295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 4591, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082295: numbers having more than two square divisors > 1
   (i.e. a(n) is the nth integer for which the number of square divisors
   strictly greater than 1 is at least 3).  The implementation tests each
   integer in increasing order, factorises it, evaluates the number of
   square divisors using the product formula
        Π (⌊e_i/2⌋ + 1)      (e_i = exponent of prime p_i),
   and keeps those that satisfy the condition.  The result is memoised
   in a dynamic array so that repeated calls for arbitrary n are fast.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* prime factorisation helper */

/* ------------------------------------------------------------------ */
/* Internal data structures used for memoisation                      */
/* ------------------------------------------------------------------ */
static seq_t *seq    = NULL;  /* array of sequence terms              */
static size_t seq_cap = 0;    /* current allocation capacity          */
static size_t seq_len = 0;    /* number of terms stored               */
static uint64_t last_tested = 0; /* largest integer that has been examined */

/* ------------------------------------------------------------------ */
/* Test whether n has at least three square divisors greater than 1.  */
/* This is equivalent to requiring Π (⌊e_i/2⌋ + 1) ≥ 4                */
/* ------------------------------------------------------------------ */
static bool is_good(uint64_t n) {
    if (n < 4) return false;  /* 4 has only two square divisors >1 */

    /* Factorise n.  For numbers ≤ 2^64 we need at most 16 prime factors. */
    uint64_t primes[16], exps[16];
    size_t   cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* Compute the product Π (⌊e_i/2⌋ + 1).  If it reaches 4 we can stop
       early to avoid overflow. */
    unsigned product = 1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned r = exps[i] >> 1;          /* ⌊e_i/2⌋ */
        product *= (r + 1);
        if (product >= 4) return true;      /* enough square divisors */
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Append a new term to the dynamic array.  Reallocate as necessary.  */
/* ------------------------------------------------------------------ */
static void add_term(seq_t val) {
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap << 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Ensure that the array contains at least 'needed' terms.  The          */
/* function searches upward from the last checked integer.               */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t needed) {
    if (seq_len >= needed) return;

    uint64_t cand = last_tested + 1;
    while (seq_len < (size_t)needed) {
        if (is_good(cand))
            add_term((seq_t)cand);
        ++cand;
    }
    last_tested = cand - 1;
}

/* ------------------------------------------------------------------ */
/* Public sequence function required by the build system.              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n < 1) return 0;            /* offset starts at 1 */
    ensure_terms(n);
    return seq[(size_t)n - 1];
}

/*
 * A015788  Numbers k such that phi(k) + 9 | sigma(k + 9).
 * https://oeis.org/A015788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015788: Numbers k such that φ(k)+9 divides σ(k+9) */
/* Implementation uses calc_phi and sigma_of utilities for exact integer arithmetic. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/calc_phi.h" /* seq_t calc_phi(seq_t m); */
#include "../utils/sigma_of.h" /* seq_t sigma_of(idx_t n); */

/* Static storage for computed terms */
static seq_t *cached_terms = NULL;   /* array of found k values */
static size_t cached_len = 0;          /* number of terms stored */
static size_t cached_cap = 0;          /* capacity of the array */
static uint64_t last_checked_k = 0;    /* largest k already tested */

/* Append a value to the cached_terms array, resizing if necessary */
static void append_term(seq_t val)
{
    if (cached_len >= cached_cap) {
        size_t new_cap = (cached_cap == 0) ? 16 : cached_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(cached_terms, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        cached_terms = new_arr;
        cached_cap = new_cap;
    }
    cached_terms[cached_len++] = val;
}

/* Expand cached array until it contains at least n terms */
static void ensure_n_terms(idx_t n)
{
    while (cached_len < (size_t)n) {
        uint64_t k = last_checked_k + 1;
        last_checked_k = k;

        /* Compute φ(k) and σ(k+9) exactly */
        seq_t phi_k = calc_phi((seq_t)k);
        seq_t sigma_k9 = sigma_of(k + 9);

        seq_t divisor = phi_k + (seq_t)9;
        /* Check divisibility: σ(k+9) % (φ(k)+9) == 0 */
        if (sigma_k9 % divisor == 0) {
            append_term((seq_t)k);
        }
    }
}

/* Public function called by harnesses */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, behavior for n<1 is unused */
    ensure_n_terms(n);             /* guarantee that a(n) exists in cache */
    return cached_terms[n - 1];    /* a(1) corresponds to cached_terms[0] */
}

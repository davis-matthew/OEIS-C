/*
 * A044173  Numbers n such that string 5,0 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_substring_50
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044173: Brute‑force search for numbers whose base‑7 representation
 * contains the substring "50" while the predecessor does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_substring_50.h"

/* dynamic array for cached terms */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;

/* state of the last value tested (has the last value contained 5,0?) */
static bool prev_has50 = false;
static seq_t next_test = 1;

/* ensure capacity for at least 'need' terms */
static void ensure_capacity(size_t need) {
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!p) exit(1);          /* out‑of‑memory */
    terms = p;
    terms_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;     /* beyond offset, undefined behaviour acceptable */
    /* continue generating terms until we have n of them */
    while (terms_len < n) {
        bool cur_has50 = has_substring_50(next_test);
        if (cur_has50 && !prev_has50) {
            ensure_capacity(terms_len + 1);
            terms[terms_len++] = next_test;
        }
        prev_has50 = cur_has50;
        ++next_test;
    }
    return terms[n - 1];
}

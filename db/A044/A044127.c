/*
 * A044127  Numbers n such that string 4,1 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: incremental search for numbers whose base‑6 representation
   contains the substring "41" while n‑1 does not. */
#include <stdbool.h>
#include <stdlib.h>

/* Helper: test whether the base‑6 representation of n contains "41". */
static bool has41(seq_t n)
{
    if (n == 0) return false;
    uint8_t digits[64];   /* enough for 128‑bit numbers (≤ 6³¹⁴)         */
    size_t len = 0;
    seq_t tmp = n;
    while (tmp != 0) {
        digits[len++] = (uint8_t)(tmp % 6);
        tmp /= 6;
    }
    /* digits[0] is least significant digit. */
    for (size_t k = 1; k < len; ++k) {
        if (digits[k] == 4 && digits[k - 1] == 1) return true;
    }
    return false;
}

/* Dynamic cache of found terms.  Static state is reused across calls. */
static seq_t *terms  = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static seq_t next_candidate = 1;   /* next number to test */

static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0) ? 64 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        if (!tmp) exit(1);   /* allocation failed */
        terms = tmp;
    }
    terms[terms_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe for caller */
    while (terms_len < n) {
        seq_t val = next_candidate;
        if (has41(val) && !has41(val - 1)) {
            append_term(val);
        }
        next_candidate++;
    }
    return terms[n - 1];
}

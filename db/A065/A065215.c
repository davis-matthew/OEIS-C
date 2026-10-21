/*
 * A065215  Numbers which need ten 'Reverse and Add' steps to reach a
 *          palindrome.
 * https://oeis.org/A065215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer requiring exactly ten
 *            reverse‑and‑add steps to reach a palindrome (palindromes
 *            excluded)
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/reverse_decimal.h"

/* Static storage for memoization */
static seq_t *seq_storage = NULL;   /* Array of found terms */
static size_t seq_buf_cap = 0;      /* Allocated capacity */
static size_t seq_len = 0;          /* Number of terms stored */
static idx_t next_candidate = 1;    /* Next integer to test */

/* Grow the storage array if needed */
static void grow_sequence(void)
{
    size_t new_cap = seq_buf_cap ? seq_buf_cap * 2 : 256;
    seq_t *new_buf = (seq_t *)realloc(seq_storage, new_cap * sizeof(seq_t));
    if (!new_buf) abort(); /* Out of memory */
    seq_storage = new_buf;
    seq_buf_cap = new_cap;
}

/* Test whether an integer requires exactly 10 reverse‑and‑add steps */
static bool requires_ten_steps(idx_t num)
{
    /* Skip palindromic starts */
    if (is_palindrome((seq_t)num, 10)) return false;

    seq_t v = (seq_t)num;
    int steps = 0;

    while (steps < 10) {
        seq_t revv = reverse_decimal(v);
        v += revv;
        ++steps;
        if (is_palindrome(v, 10)) {
            return steps == 10; /* accept only if palindrome appears at step 10 */
        }
    }
    /* No palindrome within 10 steps */
    return false;
}

/* Main function required by the interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined offset, but return 0 */

    /* Generate terms until we have at least n */
    while (seq_len < n) {
        if (requires_ten_steps(next_candidate)) {
            if (seq_len >= seq_buf_cap) grow_sequence();
            seq_storage[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_storage[n - 1];
}

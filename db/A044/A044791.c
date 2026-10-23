/*
 * A044791  Numbers n such that string 7,8 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044791: numbers n such that the decimal representation of n contains
   the substring "78" but the decimal representation of n+1 does not. Computed by
   brute‑force search with memoisation. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Test whether x contains the substring "78" in its decimal representation. */
static bool has78(seq_t x)
{
    if (x < 10) return false;
    seq_t low = x % 10;
    x /= 10;
    while (x > 0) {
        seq_t high = x % 10;
        if (low == 8 && high == 7) return true;
        low = high;
        x /= 10;
    }
    return false;
}

/* Return the n-th term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    static seq_t *memo = NULL;          /* array of found terms */
    static size_t memo_cap = 0;          /* current allocated capacity */
    static size_t memo_len = 0;          /* number of terms stored */
    static seq_t cur = 1;                /* next candidate to test */

    if (memo_cap == 0) {
        memo_cap = 64;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    }

    while (memo_len < (size_t)n) {
        if (memo_len == memo_cap) {
            memo_cap *= 2;
            memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
        }
        while (true) {
            if (has78(cur) && !has78(cur + 1)) {
                memo[memo_len++] = cur;
                cur++;  /* next search starts after this candidate */
                break;
            }
            cur++;
        }
    }

    return memo[n - 1];
}

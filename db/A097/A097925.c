/*
 * A097925  Number of (n,3) Freiman-Wyner sequences.
 * https://oeis.org/A097925
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 2 a(n-3) + 3 a(n-4) + 2 a(n-5) + a(n-6), n≥10
 *            Initial values (offset 4): a(4)=7, a(5)=12, a(6)=23, a(7)=42,
 *            a(8)=78, a(9)=142
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"

static seq_t *memo = NULL;
static size_t memo_len = 0;    /* next free index */
static size_t memo_cap = 0;    /* allocated capacity */

seq_t a(idx_t n) {
    /* return 0 for indices below the offset; behaviour is irrelevant */
    if (n < 4) return 0;

    /* ensure capacity for index n */
    if (n + 1 > memo_cap) {
        /* grow to at least n+1, but also ensure minimum capacity */
        size_t new_cap = (memo_cap < 16) ? 16 : memo_cap;
        while (new_cap < n + 1) new_cap <<= 1;
        memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        memo_cap = new_cap;
    }

    /* compute values up to n if needed */
    if (n >= memo_len) {
        size_t i = memo_len;
        for (; i <= n; ++i) {
            if (i == 4) memo[i] = (seq_t)7;
            else if (i == 5) memo[i] = (seq_t)12;
            else if (i == 6) memo[i] = (seq_t)23;
            else if (i == 7) memo[i] = (seq_t)42;
            else if (i == 8) memo[i] = (seq_t)78;
            else if (i == 9) memo[i] = (seq_t)142;
            else if (i >= 10) {
                /* recurrence: a(i) = a(i-2) + 2 a(i-3) + 3 a(i-4) + 2 a(i-5) + a(i-6) */
                seq_t term = memo[i-2]
                           + (memo[i-3] << 1)
                           + (memo[i-4] * (seq_t)3)
                           + (memo[i-5] << 1)
                           + memo[i-6];
                memo[i] = term;
            }
        }
        memo_len = n + 1;
    }

    return memo[n];
}

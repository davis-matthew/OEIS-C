/*
 * A104240  Nonnegative integers n such that 13*n^2 + 13*n + 1 is a square.
 * https://oeis.org/A104240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=7, a(2)=144, a(3)=504, a(4)=9727, a(5)=187560;
 *            a(n)=1298*a(n-3)+648-a(n-6) for n ≥ 6 This recurrence generates
 *            all non‑negative integers n such that 13*n²+13*n+1 is a perfect
 *            square. The function memoises terms, so any call order is
 *            supported
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static size_t memo_cap = 0;
static size_t memo_len = 0;

seq_t a(idx_t n)
{
    /* number of terms that must be stored (0 … n inclusive) */
    size_t target = (size_t)n + 1;

    /* Allocate or grow storage if necessary */
    if (target > memo_cap) {
        size_t new_cap = memo_cap ? memo_cap * 2 : 16;
        if (new_cap < target) new_cap = target;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed – unlikely */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Initialise the first six terms if not already done */
    if (memo_len < 6) {
        /* Ensure we have space for at least six elements */
        if (memo_cap < 6) {
            size_t new_cap = 6;
            seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            memo = tmp;
            memo_cap = new_cap;
        }
        static const seq_t init[6] = {0, 7, 144, 504, 9727, 187560};
        for (size_t i = 0; i < 6; ++i) memo[i] = init[i];
        memo_len = 6;
    }

    /* Compute any missing terms up to the requested index */
    for (size_t i = memo_len; i < target; ++i) {
        /* i ≥ 6 guarantees i-3 and i-6 are valid indices */
        seq_t val = 1298 * memo[i - 3] + 648;
        val -= memo[i - 6];
        memo[i] = val;
    }
    memo_len = target;

    return memo[n];
}

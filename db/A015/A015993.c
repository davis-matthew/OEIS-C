/*
 * A015993  Twelve iterations of Reverse and Add are needed to reach a
 *          palindrome.
 * https://oeis.org/A015993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer that requires exactly 12 reverse‑and‑add
 *            iterations to reach a palindrome. The function starts counting
 *            iterations at 1, so palindromic inputs are excluded, matching
 *            the OEIS definition of A015993
 * Range:     u8 none, u16 none, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static inline seq_t rev(seq_t x)
{
    seq_t r = 0;
    while (x) {
        r = r * (seq_t)10 + (x % (seq_t)10);
        x = x / (seq_t)10;
    }
    return r;
}

static inline bool is_pal(seq_t x)
{
    return x == rev(x);
}

static inline bool requires_12(seq_t n)
{
    seq_t cur = n;
    for (int step = 1; step <= 12; ++step) {
        cur += rev(cur);
        if (is_pal(cur))
            return step == 12;
    }
    return false;
}

static seq_t *memo = NULL;       /* store known terms */
static size_t memo_len = 0;      /* number of terms stored */
static size_t memo_cap = 0;      /* allocated capacity */
static idx_t checked_until = 0;  /* last candidate checked */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but guard */

    /* expand memo until it contains the nth term */
    while (memo_len < (size_t)n) {
        idx_t cand = checked_until + 1;
        seq_t val = (seq_t)cand;
        if (requires_12(val)) {
            /* grow array if needed */
            if (memo_len == memo_cap) {
                size_t new_cap = memo_cap ? memo_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                memo = tmp;
                memo_cap = new_cap;
            }
            memo[memo_len++] = val;
        }
        checked_until = cand;
        /* The loop will check the next candidate in the next iteration */
    }

    return memo[n - 1];
}

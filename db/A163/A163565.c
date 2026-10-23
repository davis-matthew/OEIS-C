/*
 * A163565  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 32 a(2) = 992 a(3) = 30752 a(4) = 953312 a(5) =
 *            29552176 a(n) =
 *            30*a(n-1)+30*a(n-2)+30*a(n-3)+30*a(n-4)-465*a(n-5) (n ≥ 6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *memo = NULL;
static size_t memo_cap = 0;
static size_t memo_used = 0;

/* Ensure that memo[0..n] are computed */
static void ensure_memo(idx_t n)
{
    if (n < memo_used)
        return;

    /* Grow capacity if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > memo_cap) {
        size_t new_cap = memo_cap > 0 ? memo_cap : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* cannot allocate further */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = memo_used; i <= n; ++i) {
        size_t idx = (size_t)i;
        memo[idx] = 30 * memo[idx - 1]
                  + 30 * memo[idx - 2]
                  + 30 * memo[idx - 3]
                  + 30 * memo[idx - 4]
                  - 465 * memo[idx - 5];
    }
    memo_used = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (memo == NULL) {
        /* Initialise cache and base cases */
        memo_cap  = 32;
        memo_used = 6;
        memo      = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) abort();          /* allocation failed */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)32;
        memo[2] = (seq_t)992;
        memo[3] = (seq_t)30752;
        memo[4] = (seq_t)953312;
        memo[5] = (seq_t)29552176;
    }

    ensure_memo(n);
    return memo[(size_t)n];
}

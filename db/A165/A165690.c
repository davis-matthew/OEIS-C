/*
 * A165690  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 38*(a(n-1)+a(n-2)+…+a(n-8)) – 741*a(n-9) for n≥10
 *            (coefficients come from the rational generating function).
 *            Initial values for n=0…9 are obtained from the same recurrence
 *            using the numerator coefficients: a(0)=1 a(n)=p_n +
 *            38*Σ_{k=1}^{8} a(n-k) – 741*a(n-9) (n≥1), where p_n = 1 for n=0
 *            or...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

static seq_t *memo = NULL;
static size_t memo_cap = 0;
static size_t memo_len = 0;

/* Ensure the memo array can hold indices up to n (inclusive). */
static void ensure_memo_cap(size_t n)
{
    if (n < memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 32;
    while (new_cap <= n) new_cap <<= 1;
    memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    memo_cap = new_cap;
}

/* Compute the sequence up to index n, using the linear recurrence
   derived from the generating function. */
seq_t a(idx_t n)
{
    /* Ensure we have space for up to n inclusive. */
    ensure_memo_cap((size_t)n);

    /* Compute missing terms only. */
    for (size_t i = memo_len; i <= (size_t)n; ++i)
    {
        seq_t sum = 0;

        /* Add 38 * a[i-k] for k = 1..8 if i>=k. */
        for (int k = 1; k <= 8 && i >= (size_t)k; ++k)
            sum += (seq_t)38 * memo[i - k];

        /* Subtract 741 * a[i-9] if i>=9. */
        if (i >= 9)
            sum -= (seq_t)741 * memo[i - 9];

        /* Add the RHS coefficient p_i from the numerator. */
        seq_t p = 0;
        if (i == 0 || i == 9)
            p = 1;
        else if (i >= 1 && i <= 8)
            p = 2;

        memo[i] = sum + p;
    }

    memo_len = (size_t)n + 1;
    return memo[(size_t)n];
}

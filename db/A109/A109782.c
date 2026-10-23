/*
 * A109782  Expansion of x*(1+2*x^2-2*x^3+x^4) /
 *          ((x-1)*(x^2-2*x-1)*(x^2-x+1)*(x+1)^2).
 * https://oeis.org/A109782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) + 2*a(n-2) + a(n-3) - 3*a(n-4) + 2*a(n-5) +
 *            2*a(n-6) - a(n-7) for n>6, with initial values: a(0) = 0, a(1) =
 *            1, a(2) = -2, a(3) = 8, a(4) = -21, a(5) = 54, a(6) = -134
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 24, i64 n <= 49, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* dynamic array of computed terms   */
static size_t cap = 0;            /* current allocated capacity       */
static idx_t computed_until = 0;  /* maximum index that has been computed */

/* Ensure at least n+1 elements are allocated and populated up to index n. */
static void ensure_dp(idx_t n)
{
    /* Initialize on first use. */
    if (dp == NULL) {
        cap = 8;                     /* enough for indices 0..7          */
        dp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (dp == NULL) abort();

        const seq_t init[7] = {
            (seq_t)0, (seq_t)1, (seq_t)-2, (seq_t)8,
            (seq_t)-21, (seq_t)54, (seq_t)-134
        };
        for (idx_t i = 0; i < 7; ++i)
            dp[i] = init[i];

        computed_until = 6;
    }

    if (n <= computed_until)
        return;                         /* already computed */

    /* Grow the array if necessary. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        dp = tmp;
        cap = new_cap;
    }

    /* Compute new terms. */
    for (idx_t i = computed_until + 1; i <= n; ++i) {
        dp[i] = -2 * dp[i-1] + 2 * dp[i-2] + dp[i-3]
               - 3 * dp[i-4] + 2 * dp[i-5] + 2 * dp[i-6] - dp[i-7];
    }
    computed_until = n;
}

/* External function matching the requested signature. */
seq_t a(idx_t n)
{
    if (n <= 6) {
        /* Basic values are always available once initialized. */
        if (dp == NULL) {
            /* Force initialization. */
            ensure_dp(6);
        }
        return dp[n];
    }
    ensure_dp(n);
    return dp[n];
}

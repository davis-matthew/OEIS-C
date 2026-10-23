/*
 * A029121  Expansion of 1/((1-x)(1-x^7)(1-x^9)(1-x^10)).
 * https://oeis.org/A029121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029121: Expansion of 1/((1-x)(1-x^7)(1-x^9)(1-x^10))
   Formula: a(n) = number of ways to write n as i + 7*j + 9*k + 10*l
            with i,j,k,l ≥ 0 (i is automatically determined).
   This is equivalent to the number of unordered triples (j,k,l)
   such that 7*j + 9*k + 10*l ≤ n. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *dp   = NULL;   /* dp[s] = # of triples j,k,l with 7j+9k+10l = s */
static seq_t *pref = NULL;   /* pref[s] = # of triples with 7j+9k+10l ≤ s   */
static size_t up_to = 0;     /* current maximum n that dp and pref are valid for */

/* Recompute dp and pref arrays up to at least n.  */
static void recompute_up_to(idx_t n)
{
    size_t new_size = (size_t)n + 1;
    if (new_size <= up_to)
        return;  /* already computed up to this n */

    /* Allocate new arrays */
    dp   = (seq_t *)realloc(dp,   new_size * sizeof(seq_t));
    pref = (seq_t *)realloc(pref, new_size * sizeof(seq_t));
    if (!dp || !pref) {
        /* In a production environment we should handle out‑of‑memory,
           but for the purposes of this function we simply abort. */
        abort();
    }

    /* Initialize */
    for (size_t i = 0; i < new_size; ++i)
        dp[i] = 0;
    dp[0] = 1;  /* one way to make sum 0 with no coins */

    /* Coin values: 7, 9, 10 */
    const idx_t coins[3] = {7, 9, 10};
    for (int c = 0; c < 3; ++c) {
        idx_t coin = coins[c];
        for (idx_t s = coin; s <= n; ++s) {
            dp[s] += dp[s - coin];
        }
    }

    /* Compute prefix sums: a(n) = Σ_{m=0..n} dp[m] */
    pref[0] = dp[0];
    for (size_t i = 1; i < new_size; ++i)
        pref[i] = pref[i - 1] + dp[i];

    up_to = new_size - 1;
}

seq_t a(idx_t n)
{
    recompute_up_to(n);
    return pref[(size_t)n];
}

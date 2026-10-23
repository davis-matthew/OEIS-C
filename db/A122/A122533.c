/*
 * A122533  Coefficients of the series giving the best rational approximations
 *          to 1/e.
 * https://oeis.org/A122533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(1) = 57, a(2) = 3667, a(3) = 525153. For n ≥ 4 define k = n -
 *            3 and compute a(n) = (16*k*k + 96*k + 141)*a(n-1) +
 *            ((2*k+7)*(16*k*k + 64*k + 61)/(2*k+3))*a(n-2) -
 *            ((2*k+7)/(2*k+3))*a(n-3); The recurrence is exactly the one
 *            given in the OEIS entry for A122533
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *memo = NULL;      /* memo[i] holds a(i) for i ≥ 1 */
static size_t cap = 0;          /* allocated capacity (indices 1 … cap) */
static idx_t max_computed = 0;  /* largest index whose value is known */

/* Ensure the memo table can store values up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(memo,
                                      (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        memo = tmp;
        cap   = new_cap;
    }
}

/* Core function computing the nth term of A122533. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n < 1 */

    /* Initialize the first three terms once. */
    if (max_computed == 0) {
        ensure_capacity(3);
        memo[1] = (seq_t)57;
        memo[2] = (seq_t)3667;
        memo[3] = (seq_t)525153;
        max_computed = 3;
    }

    if (n <= max_computed) return memo[n];

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        ensure_capacity(i);

        seq_t k = (seq_t)(i - 3);                /* k = n-3 at step i */

        seq_t term1 = (seq_t)16 * k * k
                    + (seq_t)96 * k
                    + (seq_t)141;

        seq_t num2 = ((seq_t)2 * k + (seq_t)7)
                   * ((seq_t)16 * k * k + (seq_t)64 * k + (seq_t)61)
                   * memo[i - 2];
        seq_t term2 = num2 / ((seq_t)2 * k + (seq_t)3);

        seq_t num3 = (seq_t)2 * k + (seq_t)7;
        seq_t term3 = (num3 * memo[i - 3]) / ((seq_t)2 * k + (seq_t)3);

        memo[i] = term1 * memo[i - 1]
                 + term2
                 - term3;
    }

    max_computed = n;
    return memo[n];
}

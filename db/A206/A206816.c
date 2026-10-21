/*
 * A206816  a(n) = Sum_{0<j<n} (n!-j!).
 * https://oeis.org/A206816
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1}^{n-1} (n! - j!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact = NULL;          /* factorials: fact[k] = k!  */
static seq_t *psum = NULL;          /* prefix sums: psum[k] = Σ_{j=1}^{k} j! */
static idx_t  max_n = 0;            /* largest n for which tables are built */

/* Extend tables up to at least n. */
static void extend_tables(idx_t n)
{
    if (n <= max_n) return;
    /* Reallocate arrays to hold indices 0..n */
    fact   = (seq_t *)realloc(fact,   sizeof(seq_t) * (n + 1));
    psum   = (seq_t *)realloc(psum,   sizeof(seq_t) * (n + 1));

    /* Initialize base case if this is the first time */
    if (max_n == 0) {
        fact[0]   = (seq_t)1;   /* 0! = 1 */
        psum[0]   = (seq_t)0;   /* no terms summed yet */
    }

    /* Fill in new entries */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;  /* i! = (i-1)! * i */
        psum[i] = psum[i - 1] + fact[i];  /* sum of j! for j=1..i */
    }
    max_n = n;
}

/* Computes a(n) for n >= 2. Behaviour for n < 2 does not matter. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined for offset, but safe */

    extend_tables(n);

    /* a(n) = (n-1) * n! - Σ_{j=1}^{n-1} j!  = (n-1) * fact[n] - psum[n-1] */
    seq_t result = (seq_t)(n - 1) * fact[n] - psum[n - 1];
    return result;
}

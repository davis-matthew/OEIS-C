/*
 * A049806  Number of Farey fractions of order n that are <=1/2; cf. A049805.
 * https://oeis.org/A049806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum_{k=2}^n (phi(k)+1)/2 where phi(k) is Euler's
 *            totient of k
 * Range:     u8 n <= 40, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes phi(k) for all k up to the
   largest n requested so far, and stores the cumulative sums
   needed to evaluate a(n).  It handles arbitrary precision
   seq_t types as long as the usual integer operators (+,-,*,/,%)
   are defined. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *phi_cache   = NULL;  /* phi values: phi_cache[i] = phi(i)  */
static seq_t *prefix_sum  = NULL;  /* prefix_sum[i] = sum_{k=2}^i (phi(k)+1)/2 */
static idx_t   max_n      = 0;     /* largest n for which tables are valid */

/* Ensure phi_cache and prefix_sum contain entries up to n. */
static void ensure_tables(idx_t n)
{
    if (n <= max_n)
        return;                       /* already up to date */

    /* Allocate / enlarge arrays */
    size_t sz = (size_t)n + 1;         /* indices 0 .. n */
    phi_cache   = (seq_t *)realloc(phi_cache, sz * sizeof(seq_t));
    prefix_sum  = (seq_t *)realloc(prefix_sum, sz * sizeof(seq_t));
    if (!phi_cache || !prefix_sum) {
        /* Allocation failure – abort (no error handling required) */
        exit(EXIT_FAILURE);
    }

    /* Allocate temporary flag array for sieve */
    bool *is_composite = (bool *)malloc(sz * sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);
    memset(is_composite, 0, sz * sizeof(bool));

    /* Initialize phi array */
    phi_cache[0] = (seq_t)0;
    phi_cache[1] = (seq_t)0;
    for (size_t i = 2; i <= (size_t)n; ++i)
        phi_cache[i] = (seq_t)i;

    /* Sieve to compute phi using the standard method */
    for (size_t p = 2; p <= (size_t)n; ++p) {
        if (!is_composite[p]) {                  /* p is prime */
            for (size_t m = p; m <= (size_t)n; m += p) {
                is_composite[m] = true;
                phi_cache[m] -= phi_cache[m] / (seq_t)p;
            }
        }
    }
    free(is_composite);

    /* Build prefix sums of (phi(k)+1)/2 for k >= 2 */
    prefix_sum[0] = (seq_t)0;
    if (n >= 1) prefix_sum[1] = (seq_t)0;
    for (size_t i = 2; i <= (size_t)n; ++i) {
        seq_t term = (phi_cache[i] + (seq_t)1) / (seq_t)2;
        prefix_sum[i] = prefix_sum[i-1] + term;
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)                /* offset starts at 1 */
        return (seq_t)0;

    ensure_tables(n);

    /* a(n) = 1 + prefix_sum[n] */
    return (seq_t)1 + prefix_sum[(size_t)n];
}

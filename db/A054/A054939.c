/*
 * A054939  Number of connected labeled graphs with n nodes and an even number
 *          of edges.
 * https://oeis.org/A054939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A001187(n)+(-1)^(n-1)*A000142(n-1))/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of connected labeled graphs with n nodes and an even number of edges. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* static seq_t binomial(idx_t a, idx_t b);   */
#include "../utils/big_pow.h"    /* static seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/factorial.h"  /* static seq_t factorial(idx_t m);           */

static seq_t *c_arr = NULL;   /* cache for A001187(n) */
static size_t c_size = 0;     /* highest n with valid c_arr[n] */

/* Ensure c_arr[n] is computed for all n up to the given value */
static void ensure_connected(idx_t n)
{
    if (n <= c_size)                /* already computed */
        return;

    /* Grow the cache array to hold indices 0..n */
    size_t new_size = (size_t)n + 1;  /* include index n */
    void *tmp = realloc(c_arr, new_size * sizeof(seq_t));
    if (!tmp)
        abort();
    c_arr = (seq_t *)tmp;

    /* Initialise any newly allocated slots to 0 */
    for (size_t i = c_size + 1; i <= (size_t)n; ++i)
        c_arr[i] = (seq_t)0;

    /* Base case for the recurrence */
    if (c_size < 1) {
        c_arr[0] = (seq_t)0;    /* unused but convenient */
        c_arr[1] = (seq_t)1;    /* exactly one connected graph on one vertex */
        c_size = 1;
    }

    /* Compute A001187(k) for k = c_size+1 ... n */
    for (idx_t i = c_size + 1; i <= n; ++i) {
        /* total number of graphs on i vertices: 2^(i*(i-1)/2) */
        idx_t total_exp = (i * (i - 1)) / 2;
        seq_t total = big_pow((seq_t)2, (seq_t)total_exp);

        seq_t sum = (seq_t)0;

        /* subtract disconnected graphs: sum over k < i */
        for (idx_t k = 1; k < i; ++k) {
            seq_t binom = binomial(i - 1, k - 1);   /* choose vertices for component */
            seq_t ck    = c_arr[k];                /* connected graphs on k vertices */

            idx_t sub_exp = ((i - k) * (i - k - 1)) / 2;
            seq_t power = big_pow((seq_t)2, (seq_t)sub_exp); /* graphs on remaining vertices */

            seq_t term = binom * ck;
            term *= power;
            sum += term;
        }

        c_arr[i] = total - sum;
    }

    c_size = (size_t)n;   /* all values up to n are now valid */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* Undefined for n < 1, return 0 for safety */

    ensure_connected(n);

    seq_t cn   = c_arr[n];             /* A001187(n) */
    seq_t fact = factorial(n - 1);     /* (n-1)! */

    seq_t result;
    if (n & 1)   /* n odd: add factorial */
        result = (cn + fact) / (seq_t)2;
    else         /* n even: subtract factorial */
        result = (cn - fact) / (seq_t)2;

    return result;
}

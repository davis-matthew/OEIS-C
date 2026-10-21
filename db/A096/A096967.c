/*
 * A096967  Bisection of A096441.
 * https://oeis.org/A096967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of 2n having all parts odd plus
 *            number of partitions of 2n having all parts even. The second
 *            term equals the ordinary partition function p(n), the first term
 *            equals the number of partitions of 2n into odd parts (denoted
 *            b_odd(2n)). Thus a(n) = p(n) + b_odd(2n), a(0)=0
 * Range:     u8 n <= 12, u16 n <= 36, u32 n <= 115, u64 n <= 391,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_odd_partitions, compute_partitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function precomputes p(k) up to the largest n requested
   and b_odd(k) up to 2n using dynamic programming with odd
   “coin” values.  The arrays are expanded only as needed. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_odd_partitions.h"
#include "../utils/compute_partitions.h"

/* The types seq_t and idx_t are provided by the build. */
static seq_t *b = NULL;            /* b_odd(k)  for k <= 2*max_n   */
static seq_t *p = NULL;            /* partition numbers p(k) for k <= max_n   */
static bool   initialized = false;
static idx_t  max_n = 0;            /* maximum n for which arrays are valid */

/* Expand internal tables to cover at least n. */
static void ensure_tables(idx_t n)
{
    if (!initialized || n > max_n) {
        idx_t new_max_n = n;
        idx_t new_max_2n = 2 * new_max_n;

        seq_t *new_b = (seq_t *)calloc(new_max_2n + 1, sizeof(seq_t));
        seq_t *new_p = (seq_t *)calloc(new_max_n + 1,   sizeof(seq_t));

        compute_partitions(new_max_n, new_p);      /* p(k) for k <= n        */
        compute_odd_partitions(new_max_2n, new_b); /* b_odd(k) for k <= 2n  */

        free(b);
        free(p);
        b = new_b;
        p = new_p;
        max_n = new_max_n;
        initialized = true;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) is defined as 0 explicitly */
    ensure_tables(n);
    return b[2 * n] + p[n];
}

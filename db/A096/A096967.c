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
 * Validated: standard, pass, 61 terms through n = 997 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function precomputes p(k) up to the largest n requested
   and b_odd(k) up to 2n using dynamic programming with odd
   “coin” values.  The arrays are expanded only as needed. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to compute b_odd[0..M] where M = 2*max_n
   using the standard coin change DP with odd coins. */
static void compute_odd_partitions(idx_t M, seq_t *out)
{
    out[0] = 1;
    for (idx_t coin = 1; coin <= M; coin += 2) {
        for (idx_t t = coin; t <= M; ++t) {
            out[t] += out[t - coin];
        }
    }
}

/* Helper to compute the partition numbers p[0..N] using the
   generalized pentagonal number theorem. */
static void compute_partitions(idx_t N, seq_t *out)
{
    out[0] = 1;
    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (int k = 1; ; ++k) {
            idx_t pent1 = (idx_t)k * ((idx_t)3 * k - 1) / 2;
            if (pent1 > n) break;
            seq_t sign = (k & 1) ? 1 : -1;
            sum += sign * out[n - pent1];
            idx_t pent2 = (idx_t)k * ((idx_t)3 * k + 1) / 2;
            if (pent2 <= n) sum += sign * out[n - pent2];
        }
        out[n] = sum;
    }
}

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

        seq_t *new_b = mem_new(seq_t, new_max_2n + 1);
        seq_t *new_p = mem_new(seq_t, new_max_n + 1);

        compute_partitions(new_max_n, new_p);      /* p(k) for k <= n        */
        compute_odd_partitions(new_max_2n, new_b); /* b_odd(k) for k <= 2n  */

        mem_free(b);
        mem_free(p);
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

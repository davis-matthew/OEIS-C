/*
 * A039903  Number of partitions satisfying 0 < cn(0,5) + cn(1,5) + cn(2,5) +
 *          cn(3,5) and 0 < cn(0,5) + cn(4,5) + cn(2,5) + cn(3,5).
 * https://oeis.org/A039903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) – p₁(n) – p₄(n) where: p(n) = number of unrestricted
 *            partitions of n, p₁(n) = number of partitions of n using only
 *            parts ≡ 1 (mod 5), p₄(n) = number of partitions of n using only
 *            parts ≡ 4 (mod 5)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 892 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 0, but a(0) = 0 by convention.
       The dynamic-programming tables below count the empty partition
       as a valid partition, so we adjust the result for n == 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Allocate tables of size n + 1 */
    size_t N = (size_t)(n + 1);
    seq_t *p  = alloc_seq_array(N);   /* unrestricted partitions   */
    seq_t *p1 = alloc_seq_array(N);   /* parts ≡ 1 (mod 5) only     */
    seq_t *p4 = alloc_seq_array(N);   /* parts ≡ 4 (mod 5) only     */

    /* Initialize to zero and set base case (partition of 0 = 1) */
    for (idx_t i = 0; i <= n; ++i) {
        p[i]  = 0;
        p1[i] = 0;
        p4[i] = 0;
    }
    p[0]  = 1;
    p1[0] = 1;
    p4[0] = 1;

    /* Unrestricted partitions: classic coin‑change DP */
    for (idx_t s = 1; s <= n; ++s) {
        for (idx_t j = s; j <= n; ++j) {
            p[j] += p[j - s];
        }
    }

    /* Partitions with all parts ≡ 1 (mod 5) */
    for (idx_t s = 1; s <= n; s += 5) {
        for (idx_t j = s; j <= n; ++j) {
            p1[j] += p1[j - s];
        }
    }

    /* Partitions with all parts ≡ 4 (mod 5) */
    for (idx_t s = 4; s <= n; s += 5) {
        for (idx_t j = s; j <= n; ++j) {
            p4[j] += p4[j - s];
        }
    }

    seq_t result = p[n];
    result -= p1[n];
    result -= p4[n];

    /* Clean up allocated memory */
    free_seq_array(p);
    free_seq_array(p1);
    free_seq_array(p4);

    return result;
}

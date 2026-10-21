/*
 * A089820  Number of subsets of {1,..,n} containing at least one prime.
 * https://oeis.org/A089820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 2^(n-π(n)) (number of non‑empty subsets of {1,…,n}
 *            that contain at least one prime)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   prime_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/prime_count.h"

/* Compute 2^k as a seq_t by repeated multiplication.   */
static seq_t pow2(size_t k)
{
    seq_t r = 1;
    while (k--) r *= (seq_t)2;
    return r;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;                    /* sequence is defined for n≥1 */

    uint64_t pi_n = prime_count(n);

    seq_t total_subsets   = pow2((size_t)n);           /* 2^n            */
    seq_t nonprime_subsets = pow2((size_t)(n - pi_n)); /* 2^(n-π(n))      */

    return total_subsets - nonprime_subsets;
}

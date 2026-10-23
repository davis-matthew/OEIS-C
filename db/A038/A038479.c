/*
 * A038479  Sums of 3 distinct powers of 6.
 * https://oeis.org/A038479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th smallest integer whose * base‑6 representation
 *            contains exactly three 1's * and all other digits 0 (sums of 3
 *            distinct powers of 6)
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 286, u64 n <= 2300,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 6^e as a seq_t using repeated multiplication */
static seq_t pow6(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= (seq_t)6;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset not defined, but safe handling */

    seq_t N = (seq_t)n;                    /* promote to seq_t for calculations */
    idx_t k = 2;
    seq_t cumulative = 0;

    /* Find the maximum exponent k such that the number of sums with
     * largest exponent <= k reaches or exceeds n. */
    for (;; ++k) {
        seq_t add = ((seq_t)k * (seq_t)(k - 1)) / 2;   /* C(k,2) */
        if (cumulative + add >= N)
            break;
        cumulative += add;
    }

    /* Offset of the desired sum within the group with maximum exponent k */
    seq_t remaining = N - cumulative - 1;            /* 0‑based index */
    idx_t offset = (idx_t)remaining;

    /* Find the j parameter (the middle exponent) by subtracting group sizes */
    idx_t j = 1;
    for (;;) {
        if (offset < (idx_t)j)
            break;
        offset -= j;
        ++j;
    }

    idx_t i = offset;                 /* smallest exponent */

    /* Evaluate the sum 6^i + 6^j + 6^k */
    seq_t sum = pow6(i) + pow6(j) + pow6(k);
    return sum;
}

/*
 * A238946  Maximal level size of arcs in divisor lattice D(n).
 * https://oeis.org/A238946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal sum of in-degrees on any level of the divisor
 *            lattice of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, enumerates all divisors by recursion, accumulates
 * the sum of in-degrees for each level (bigomega value), and returns the maximum. */

#include <stdint.h>
#include <stdlib.h>

#define MAX_FACTORS 64

static void dfs_divisors(int idx, int k, const unsigned int exp[MAX_FACTORS],
                         unsigned int bigomega, unsigned int omega, seq_t *sums)
{
    if (idx == k) {
        sums[bigomega] += (seq_t)omega;
        return;
    }
    for (unsigned int e = 0; e <= exp[idx]; ++e) {
        dfs_divisors(idx + 1, k, exp, bigomega + e,
                     omega + (e ? 1 : 0), sums);
    }
}

seq_t a(idx_t n)
{
    /* Factor n into primes -> exponents array */
    uint64_t temp = (uint64_t)n;
    unsigned int exp[MAX_FACTORS];
    int k = 0;

    if (temp % 2 == 0) {
        unsigned int e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++e;
        }
        exp[k++] = e;
    }
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            exp[k++] = e;
        }
    }
    if (temp > 1) {
        exp[k++] = 1;          /* remaining prime factor has exponent 1 */
    }

    /* Sum of all exponents = maximum possible bigomega */
    unsigned int sum_exp = 0;
    for (int i = 0; i < k; ++i) sum_exp += exp[i];

    /* Allocate array for sums per level */
    seq_t *sums = (seq_t *)calloc((size_t)sum_exp + 1, sizeof(seq_t));
    if (!sums) return 0;        /* Allocation failure, return 0 as safe fallback */

    /* Enumerate all divisors */
    dfs_divisors(0, k, exp, 0, 0, sums);

    /* Find maximum sum over all levels */
    seq_t max_val = 0;
    for (unsigned int i = 0; i <= sum_exp; ++i) {
        if (sums[i] > max_val) max_val = sums[i];
    }

    free(sums);
    return max_val;
}

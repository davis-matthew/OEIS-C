/*
 * A179983  Positive integers m such that, if k appears in m's prime
 *          signature, k-1 appears at least as often as k (for any integer k >
 *          1).
 * https://oeis.org/A179983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration: finds the n‑th integer m whose prime signature exponents
   satisfy c_k <= c_{k-1} for all integers k > 1 (where c_k is the
   number of times exponent k occurs in the signature). */

/* Return the n-th term of OEIS A179983.  Index n starts at 1. */
#include <stdint.h>
#include <stdlib.h>

#define MAX_EXP 64          /* maximum possible exponent for 64‑bit values */

static inline int is_valid(uint64_t m)
{
    if (m == 1) return 1;      /* 1 has an empty signature and is valid */

    uint64_t tmp = m;
    int exp_count[MAX_EXP + 1] = {0};
    int max_exp = 0;

    /* count exponent of 2 */
    if ((tmp & 1ULL) == 0) {
        int e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            e++;
        }
        exp_count[e]++;
        if (e > max_exp) max_exp = e;
    }

    /* count exponents of odd primes */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            exp_count[e]++;
            if (e > max_exp) max_exp = e;
        }
    }

    /* any remaining factor is a prime > sqrt(tmp) */
    if (tmp > 1) {
        exp_count[1]++;
        if (1 > max_exp) max_exp = 1;
    }

    /* verify the non‑increasing count condition */
    int last = exp_count[1];   /* count of exponent 1 */
    for (int k = 2; k <= max_exp; k++) {
        if (exp_count[k] > last) return 0;
        last = exp_count[k];
    }
    return 1;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t m = 1;
    while (found < n) {
        if (is_valid(m)) {
            found++;
            if (found == n)
                return (seq_t)m;
        }
        m++;
    }
    /* unreachable, but keep the compiler happy */
    return (seq_t)m;
}

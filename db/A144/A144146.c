/*
 * A144146  A positive integer k is included if every nonzero exponent in the
 *          prime factorization of k is coprime to k.
 * https://oeis.org/A144146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k-th positive integer such that every non‑zero exponent
 *            in its prime factorisation is coprime to k. (Computed by linear
 *            search.)
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static bool passes(uint64_t k)
{
    if (k == 1) return true;          /* 1 is included by definition */

    uint64_t n = k;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int exp = 0;
            do {
                n /= p;
                ++exp;
            } while (n % p == 0);
            /* exp is the exponent of prime p */
            seq_t g = gcd((seq_t)exp, (seq_t)k);
            if (g != (seq_t)1) return false;
        }
    }
    /* Remaining factor (if >1) has exponent 1, which is always coprime to k */
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t count = 0;
    uint64_t candidate = 1;

    while (true) {
        if (passes(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}

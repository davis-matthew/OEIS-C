/*
 * A085072  Smallest k such that n and n+k have the same prime signature.
 * https://oeis.org/A085072
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 9962, u64 n <= 9962,
 *            u128 n <= 9962, big any n
 * Helpers:   factor_u64, sorted_exponents
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A085072:  a(n) = smallest k>0 such that n and n+k have the same
 * prime‑signature (multiset of exponents in the prime factorisation).
 * Uses trial factoring via factor_u64 from "../utils/factor_u64.h". */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/sorted_exponents.h"

/* Compares two sorted signatures for equality. */
static bool same_signature(const uint8_t *a, size_t ca,
                           const uint8_t *b, size_t cb)
{
    if (ca != cb) return false;
    for (size_t i = 0; i < ca; ++i)
        if (a[i] != b[i]) return false;
    return true;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* below offset – value ignored */

    /* factorise n */
    uint64_t primes1[64];
    uint64_t exps1[64];
    size_t cnt1 = 0;
    factor_u64((uint64_t)n, primes1, exps1, &cnt1);

    /* build sorted signature of n */
    uint8_t sig1[64];
    sorted_exponents(exps1, cnt1, sig1);

    /* search forward for smallest k with same signature */
    for (uint64_t m = (uint64_t)n + 1; ; ++m) {
        uint64_t primes2[64];
        uint64_t exps2[64];
        size_t cnt2 = 0;
        factor_u64(m, primes2, exps2, &cnt2);

        uint8_t sig2[64];
        sorted_exponents(exps2, cnt2, sig2);

        if (same_signature(sig1, cnt1, sig2, cnt2))
            return (seq_t)(m - (uint64_t)n);
    }
}

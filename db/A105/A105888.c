/*
 * A105888  a(n) = the smallest prime that, when written in binary, ends with
 *          the substring of 2n-1 in binary.
 * https://oeis.org/A105888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest prime whose binary representation ends with
 *            the binary representation of (2*n-1). The algorithm tests
 *            candidates of the form p = t + k*(2^ℓ) where t = 2*n-1 and ℓ =
 *            bit-length(t)
 * Range:     u8 n <= 27, u16 n <= 1092, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_uint64, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime_uint64.h"   /* use the provided uint64 tester */
#include "../utils/bitlen_uint64.h"

seq_t a(idx_t n) {
    /* This function satisfies the external linkage requirement. */
    if (n == 0) {
        /* undefined for offset < 1; return 0 as a safe placeholder */
        return (seq_t)0;
    }

    uint64_t nn = (uint64_t)n;          /* cast idx_t to 64‑bit uint */
    uint64_t t  = 2 * nn - 1;           /* t = 2*n-1 (odd, >=1) */

    /* Calculate ℓ = bit-length(t) */
    int ell = bitlen_uint64(t);          /* 1 <= ell <= 64 for t>=1 */

    /* Compute step = 2^ℓ.  For ell == 64 the shift would overflow,
       but n is assumed to stay within a range where ell <= 63. */
    uint64_t step = 1ULL << ell;

    uint64_t candidate = t;
    while (!is_prime_uint64(candidate)) {
        candidate += step;
    }

    return (seq_t)candidate;
}

/*
 * A140387  Binary encoding of the location of primes in integer sets r+30*n
 *          with remainder r=1,7,11,..,29.
 * https://oeis.org/A140387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary number whose k‑th bit (k=0…7) is 1 iff
 *            30*(n‑1)+r_k is composite, where the remainders r_k are
 *            {1,7,11,13,17,19,23,29}. The resulting number is returned as
 *            seq_t
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    if ((n & 1ULL) == 0) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* Fixed order of remainders corresponding to bit positions 0..7 */
    static const uint64_t rem[8] = {1ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL};

    /* Compute the base 30*(n-1) in 64‑bit to avoid overflow for small seq_t. */
    uint64_t base = (uint64_t)(n - 1) * 30ULL;

    seq_t result = 0;
    for (int k = 0; k < 8; ++k) {
        uint64_t cand = base + rem[k];
        if (!is_prime_u64(cand))
            result |= (seq_t)1 << k;
    }
    return result;
}

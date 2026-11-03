/*
 * A039634  Fixed point of "n -> n/2 or (n-1)/2 until result is prime".
 * https://oeis.org/A039634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = largest prime whose binary expansion is an initial *
 *            substring of n's binary expansion. a(1) = 1 by convention
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9852 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Return the n-th term of OEIS A039634 (sample index starts at 1). */
seq_t a(idx_t n) {
    if (n == 0) return 1;               /* safety, not used for n >= 1   */

    /* number of bits in n */
    idx_t tmp = n;
    size_t len = 0;
    while (tmp) {
        len++;
        tmp >>= 1;
    }

    seq_t best = 1;            /* convention for cases with no prime prefix */
    seq_t pref = 0;

    /* build prefixes from most significant bit to least */
    for (size_t i = 0; i < len; ++i) {
        pref = (pref << 1) | ((n >> (len - 1 - i)) & 1ULL);
        if (isPrimeGeneric(pref) && pref > best)
            best = pref;
    }
    return best;
}

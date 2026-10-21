/*
 * A086107  Prime members of A086108: Prime numbers which have the additional
 *          property that all symmetric polynomials of their digits are also
 *          prime numbers.
 * https://oeis.org/A086107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the nth prime whose elementary symmetric sums of its
 *            base‑10 digits are all prime. The sequence is finite (only the
 *            eight numbers listed are found)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static bool check_property(uint64_t x) {
    /* Check that all elementary symmetric sums of the digits of x are prime. */
    /* Extract digits */
    uint8_t digits[20];
    int dlen = 0;
    uint64_t tmp = x;
    if (tmp == 0) {
        digits[dlen++] = 0;
    } else {
        while (tmp > 0) {
            digits[dlen++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }
    /* Compute sums of products of all non‑empty subsets of digits. */
    uint64_t sums[21] = {0};   /* sums[sz] accumulates products of subsets of size sz */
    int maxmask = 1 << dlen;
    for (int mask = 1; mask < maxmask; ++mask) {
        int sz = __builtin_popcount(mask);
        uint64_t prod = 1;
        for (int i = 0; i < dlen; ++i)
            if (mask & (1 << i))
                prod *= digits[i];
        sums[sz] += prod;
    }
    /* All sums must be prime. */
    for (int sz = 1; sz <= dlen; ++sz) {
        if (!is_prime_small(sums[sz]))
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t term[16];   /* the sequence is known to have at most 8 terms */
    static size_t len = 0;

    if (!initialized) {
        /* Generate primes up to a reasonable limit and collect the ones that
           satisfy the property. No term is known beyond 311, but we search
           up to one million to be safe. */
        const uint64_t LIMIT = 1000000;
        for (uint64_t p = 2; p <= LIMIT; ++p) {
            if (!is_prime_small(p))
                continue;
            if (check_property(p)) {
                term[len++] = (seq_t)p;
                /* We expect only 8 terms; break early once found. */
                if (len >= 8)
                    break;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > len)
        return (seq_t)0;   /* out of bounds – sequence is finite */

    return term[n - 1];
}

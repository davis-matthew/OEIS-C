/*
 * A090334  Numbers with at least one divisor >1 that is a prefix of another
 *          divisor in binary representation.
 * https://oeis.org/A090334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A090334.
   The sequence consists of numbers n>2 that have a divisor d>1 which is a
   prefix of another divisor of n when both divisors are written in binary.
   Every even number >2 satisfies the property.  For odd numbers
   the property is tested by enumerating all divisors >1. */

#include <stdint.h>
#include <stdbool.h>

static inline uint8_t bitlen_uint64(uint64_t v) {
    uint8_t b = 0;
    while (v) {
        v >>= 1;
        b++;
    }
    return b;
}

/* Return true if m (>2) satisfies the property.  Even m>2 always return true. */
static bool satisfies(uint64_t m) {
    if ((m & 1u) == 0 && m > 2)
        return true;   /* all even numbers >2 are in the sequence */

    /* collect divisors >1 */
    uint64_t divs[64];
    uint8_t dcnt = 0;
    divs[dcnt++] = m;                     /* m itself */

    for (uint64_t j = 2; j * j <= m; ++j) {
        if (m % j == 0) {
            divs[dcnt++] = j;
            uint64_t d2 = m / j;
            if (d2 != j)
                divs[dcnt++] = d2;
        }
    }

    /* test all unordered pairs of distinct divisors */
    for (uint8_t i = 0; i < dcnt; ++i) {
        uint8_t len_i = bitlen_uint64(divs[i]);
        for (uint8_t j = 0; j < dcnt; ++j) {
            if (i == j)
                continue;
            uint8_t len_j = bitlen_uint64(divs[j]);
            if (len_j >= len_i) {
                if ((divs[j] >> (len_j - len_i)) == divs[i])
                    return true;
            }
        }
    }
    return false;
}

seq_t a(idx_t n) {
    uint64_t candidate = 3;   /* start just below the first term (4) */
    idx_t     found = 0;

    while (1) {
        candidate++;
        if (candidate > 2 && satisfies(candidate)) {
            found++;
            if (found == n)
                return (seq_t)candidate;
        }
    }
}

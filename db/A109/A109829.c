/*
 * A109829  Number of distinct prime factors of 149 - (n-th prime power less
 *          than 149).
 * https://oeis.org/A109829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of distinct prime factors of 149 - A000961(n), *
 *            where A000961(n) is the n‑th prime power (including 1) less than
 *            149
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Prime powers (p^k) less than 149, enumerated in ascending order
 * (counting 1 as the first prime power). */
static const uint32_t PRIME_POWERS[] = {
    1,  2,  3,  4,  5,  7,  8,  9,  11, 13, 16, 17, 19, 23,
    25, 27, 29, 31, 32, 37, 41, 43, 47, 49, 53, 59, 61, 64,
    67, 71, 73, 79, 81, 83, 89, 97, 101,103,107,109,113,
    121,125,127,128,131,137,139
};
static const size_t PRIME_POWER_COUNT = sizeof(PRIME_POWERS) / sizeof(PRIME_POWERS[0]);

/* Small primes used for factorisation of numbers <= 149. */
static const uint32_t PRIME_LIST[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
    43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89,
    97,101,103,107,109,113,127,131,137,139
};
static const size_t PRIME_COUNT = sizeof(PRIME_LIST) / sizeof(PRIME_LIST[0]);

seq_t a(idx_t n)
{
    /* Index 1 refers to the first prime power (which is 1). */
    if (n == 0) {
        return (seq_t)0;          /* undefined for offset 1 */
    }

    /* Translate 1‑based OEIS index to 0‑based array index. */
    idx_t idx = (idx_t)(n - 1);

    /* Sequence ends after 48 terms (prime powers < 149). */
    if ((size_t)idx >= PRIME_POWER_COUNT) {
        return (seq_t)0;
    }

    /* Difference 149 - (n‑th prime power). */
    uint32_t diff = 149u - PRIME_POWERS[(size_t)idx];
    if (diff <= 1u) {          /* 0 or 1 has no prime factors */
        return (seq_t)0;
    }

    /* Count distinct prime factors of diff. */
    seq_t count = 0;
    uint32_t remaining = diff;

    for (size_t i = 0; i < PRIME_COUNT; ++i) {
        uint32_t p = PRIME_LIST[i];
        if (p * p > remaining) {
            break;
        }
        if (remaining % p == 0u) {
            ++count;
            do {
                remaining /= p;
            } while (remaining % p == 0u);
        }
    }
    if (remaining > 1u) {   /* Remaining part is a prime factor > sqrt(original) */
        ++count;
    }

    return count;
}

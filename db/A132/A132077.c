/*
 * A132077  Numbers having in decimal representation exactly the same number
 *          of distinct digits as number of divisors.
 * https://oeis.org/A132077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 7765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A132077: a(n) = the n‑th positive integer whose decimal
 * representation contains exactly as many distinct digits as the
 * number of its positive divisors. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Count distinct decimal digits in a uint64_t (0 → 1 digit). */
static inline uint8_t distinct_digit_count(uint64_t x)
{
    unsigned mask = 0;
    if (x == 0) return 1;
    while (x) {
        mask |= 1U << (x % 10);
        x /= 10;
    }
#if defined(__GNUC__) || defined(__clang__)
    return (uint8_t)__builtin_popcount(mask);
#else
    uint8_t cnt = 0;
    while (mask) {
        cnt += mask & 1U;
        mask >>= 1;
    }
    return cnt;
#endif
}

/* Count number of positive divisors of a uint64_t via trial division. */
static inline uint64_t divisor_count_uint64(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t res = 1;
    uint64_t tmp = n;

    /* Handle factor 2 separately. */
    if ((tmp & 1) == 0) {
        int e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++e;
        }
        res *= (uint64_t)(e + 1);
    }

    /* Odd factors. */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            res *= (uint64_t)(e + 1);
        }
    }

    if (tmp > 1) res *= 2ULL;   /* Remaining prime factor. */
    return res;
}

/* The sequence function.  idx_t is an unsigned integer type. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined below offset */

    uint64_t found = 0;
    for (uint64_t cand = 1;; ++cand) {
        uint8_t dcnt_digits = distinct_digit_count(cand);
        uint64_t dcnt_divisors = divisor_count_uint64(cand);
        if (dcnt_digits == dcnt_divisors) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
    }
}

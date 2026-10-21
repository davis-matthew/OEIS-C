/*
 * A076721  Difference between product of divisors of n and sum of divisors of
 *          n.
 * https://oeis.org/A076721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = product of divisors of n – sum of divisors of n
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 59, i64 n <= 179,
 *            i128 n <= 719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root (floor) for uint64_t */
static inline uint64_t isqrt_uint64(uint64_t x) {
    uint64_t low = 0, high = x, res = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (mid == 0) {          /* guard for division by zero */
            res = 0;
            low = 1;
            continue;
        }
        if (mid <= x / mid) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n) {
    /* n is guaranteed to be >= 1 */
    uint64_t m = (uint64_t)n;
    uint64_t tau = 1;          /* count of divisors */
    seq_t sigma = 1;           /* sum of divisors */

    /* factorisation by trial division */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            seq_t part = 1;    /* sum of p^0 + p^1 + ... + p^exp */
            seq_t power = 1;   /* current p^k */
            while (m % p == 0) {
                m /= p;
                exp++;
                power *= (seq_t)p;   /* next power of p */
                part += power;
            }
            tau *= (exp + 1);
            sigma *= part;
        }
    }
    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t part = 1 + (seq_t)m; /* p^0 + p^1 */
        tau *= 2;
        sigma *= part;
    }

    /* product of divisors */
    seq_t prod;
    if ((tau & 1ULL) == 0) {          /* tau even */
        idx_t exp = (idx_t)(tau / 2);
        prod = 1;
        for (idx_t i = 0; i < exp; ++i) {
            prod *= (seq_t)n;
        }
    } else {                          /* tau odd – perfect square */
        uint64_t sqrt_n = isqrt_uint64((uint64_t)n);
        idx_t exp = (idx_t)((tau - 1) / 2);
        prod = 1;
        for (idx_t i = 0; i < exp; ++i) {
            prod *= (seq_t)n;
        }
        prod *= (seq_t)sqrt_n;
    }

    return prod - sigma;
}

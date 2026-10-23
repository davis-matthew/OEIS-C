/*
 * A106889  Primes of the form 2x^2 + 5y^2.
 * https://oeis.org/A106889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1646, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 2x^2 + 5y^2 (OEIS A106889)
   The function a(n) returns the n-th such prime (n≥1).  It is found by
   exhaustive search: testing candidates in increasing order, checking
   primality (by trial division) and the representation condition. */

#include <stdint.h>
#include <stdbool.h>

/* ----- integer square root for uint64_t ----- */
static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t r = 0;
    for (uint64_t bit = (uint64_t)1 << 62; bit; bit >>= 2) {
        uint64_t next = r + bit;
        if (n >= next) {
            n -= next;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

/* ----- primality test for uint64_t (trial division) ----- */
static inline bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL) {
        if (n % d == 0ULL) return false;
    }
    return true;
}

/* ----- check whether p can be written as 2x^2 + 5y^2 ----- */
static inline bool rep_by_2x2plus5y2(uint64_t p) {
    if (p == 2ULL || p == 5ULL) return true;          /* trivial cases */

    uint64_t max_y = isqrt_u64(p / 5ULL);
    for (uint64_t y = 1ULL; y <= max_y; ++y) {
        uint64_t y2 = y * y;          /* y^2  (fits in 64 bits) */
        uint64_t rem = p - 5ULL * y2; /* p – 5y^2 */
        if (rem & 1ULL) continue;     /* must be even */
        uint64_t x2 = rem >> 1ULL;    /* (p – 5y^2)/2 = x^2 */
        uint64_t xsq = isqrt_u64(x2);
        if (xsq * xsq == x2) return true; /* integer x found */
    }
    return false;
}

/* ----- sequence A106889 ----- */
seq_t a(idx_t n) {
    if (n == 0) return 0;    /* undefined for offset 0, harmless */

    uint64_t count = 0;      /* number of primes of the required form found */
    uint64_t p = 2ULL;       /* start from the first prime */

    while (true) {
        if (is_prime_u64(p) && rep_by_2x2plus5y2(p)) {
            ++count;
            if (count == n) {
                return (seq_t)p;
            }
        }
        /* advance to the next candidate: skip even numbers after 2 */
        if (p == 2ULL) {
            p = 3ULL;
        } else {
            p += 2ULL;
        }
    }
}

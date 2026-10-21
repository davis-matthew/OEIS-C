/*
 * A218654  Number of ways to write n as x+y with 0<x<=y and x^2+3xy+y^2
 *          prime.
 * https://oeis.org/A218654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of x,y with 0 < x <= y, x + y = n and x^2 + 3xy +
 *            y^2 is prime
 * Range:     u8 n <= 1990, u16 n <= 9446, u32 n <= 9446, u64 n <= 9446,
 *            u128 n <= 9446, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* simple integer square‑root (floor) for 64‑bit values */
static inline uint64_t isqrt_uint64(uint64_t v) {
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;  /* largest power of 4 <= 2^64 */
    while (bit > v) bit >>= 2;
    while (bit) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* primes up to 12000 – enough for the square‑root of the largest test value (~1.2×10^4) */
static bool primes_init = false;
static uint64_t primes[2000];
static size_t prime_cnt = 0;

static void init_primes(void) {
    if (primes_init) return;
    const size_t LIM = 12000;
    bool is_prime[LIM + 1];
    for (size_t i = 0; i <= LIM; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= LIM; ++i)
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIM; j += i) is_prime[j] = false;
        }
    for (size_t i = 2; i <= LIM; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;
    primes_init = true;
}

/* primality test using the pre‑computed prime table */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    uint64_t sq = isqrt_uint64(n);
    for (size_t i = 0; i < prime_cnt && primes[i] <= sq; ++i)
        if (n % primes[i] == 0) return false;
    return true;
}

/* required function */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    init_primes();
    size_t count = 0;
    idx_t max_x = n / 2;               /* iterate x from 1 to floor(n/2) */
    for (idx_t x = 1; x <= max_x; ++x) {
        idx_t y = n - x;               /* y >= x automatically */
        uint64_t sx = (uint64_t)x;
        uint64_t sy = (uint64_t)y;
        uint64_t val = sx * sx + 3 * sx * sy + sy * sy;   /* x^2+3xy+y^2 */
        if (is_prime_uint64(val)) ++count;
    }
    return (seq_t)count;
}

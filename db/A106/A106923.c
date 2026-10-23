/*
 * A106923  Primes of the form 4x^2+xy+4y^2, with x and y nonnegative.
 * https://oeis.org/A106923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime representable by the form 4x^2 + xy + 4y^2
 *            (x,y ≥ 0)
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* 128-bit integer square root.  */
static uint64_t isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = (unsigned __int128)1 << 64;  /* sqrt(2^128-1) < 2^64 */
    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x) low = mid + 1;
        else         high = mid;
    }
    return (uint64_t)(low - 1);
}

/* Test whether a 64‑bit integer is a prime (trial division by known primes).  */
static bool is_prime_candidate(uint64_t cand, uint64_t *primes, size_t primes_len)
{
    if (cand < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p > cand / p) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* Test whether a prime p is representable by 4x^2 + xy + 4y^2.  */
static bool representable(uint64_t p)
{
    /* Minimum represented prime is 43.  */
    if (p < 43u) return false;

    /* Upper bound for y: 4y^2 <= p  ==>  y <= sqrt(p/4).  */
    uint64_t y_max = (uint64_t)isqrt128((unsigned __int128)p / 4);

    for (uint64_t y = 0; y <= y_max; ++y) {
        unsigned __int128 y2   = (unsigned __int128) y * y;
        unsigned __int128 d128 = (unsigned __int128)16 * p - (unsigned __int128)63 * y2;
        if (d128 == 0) break;           /* further y will make d128 negative */
        uint64_t sqrtD = isqrt128(d128);
        if ((unsigned __int128)sqrtD * sqrtD != d128) continue;
        uint64_t num = sqrtD - y;        /* numerator for x */
        if (num % 8 == 0) return true;  /* integer x >= 0 follows automatically */
    }
    return false;
}

/* Dynamic array helpers.  */
static struct {
    seq_t  *vals;      /* primes of the form 4x^2 + xy + 4y^2  */
    size_t  len;
    size_t  cap;
} seq_state = {NULL, 0, 0};

static struct {
    uint64_t *primes;  /* all primes discovered, used for primality testing */
    size_t   len;
    size_t   cap;
} prime_state = {NULL, 0, 0};

static uint64_t current_candidate = 2;  /* next integer to examine */

/* Ensure capacity for seq_state.vals.  */
static void ensure_seq_capacity(void)
{
    if (seq_state.cap == 0) {
        seq_state.cap = 128;
        seq_state.vals = (seq_t *)malloc(seq_state.cap * sizeof(seq_t));
    } else if (seq_state.len == seq_state.cap) {
        seq_state.cap <<= 1;
        seq_state.vals = (seq_t *)realloc(seq_state.vals, seq_state.cap * sizeof(seq_t));
    }
}

/* Ensure capacity for prime_state.primes.  */
static void ensure_prime_capacity(void)
{
    if (prime_state.cap == 0) {
        prime_state.cap = 128;
        prime_state.primes = (uint64_t *)malloc(prime_state.cap * sizeof(uint64_t));
    } else if (prime_state.len == prime_state.cap) {
        prime_state.cap <<= 1;
        prime_state.primes = (uint64_t *)realloc(prime_state.primes, prime_state.cap * sizeof(uint64_t));
    }
}

/* Main function requested by the build system.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Keep generating primes until we have at least n terms.  */
    while (seq_state.len < (size_t)n) {
        if (is_prime_candidate(current_candidate, prime_state.primes, prime_state.len)) {
            /* Store prime for future primality tests.  */
            ensure_prime_capacity();
            prime_state.primes[prime_state.len++] = current_candidate;

            if (representable(current_candidate)) {
                ensure_seq_capacity();
                seq_state.vals[seq_state.len++] = (seq_t)current_candidate;
            }
        }
        ++current_candidate;
    }
    return seq_state.vals[n - 1];
}

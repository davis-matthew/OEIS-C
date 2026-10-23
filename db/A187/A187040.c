/*
 * A187040  Numbers for which Midy's theorem holds.
 * https://oeis.org/A187040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: scan integers and test the Midy's property. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t modpow10(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

static bool good_candidate(uint64_t n)
{
    /* remove all factors 2 and 5 */
    while ((n & 1ULL) == 0) n >>= 1;
    while (n % 5ULL == 0) n /= 5;
    if (n <= 1ULL) return false;   /* pure 2^a5^b are excluded */

    /* compute Euler's totient φ(n) */
    uint64_t tmp = n;
    uint64_t phi = n;

    if (tmp % 2ULL == 0) {
        phi -= phi / 2;
        while (tmp % 2ULL == 0) tmp /= 2;
    }

    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            phi -= phi / p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1ULL) phi -= phi / tmp;

    /* compute multiplicative order of 10 modulo n */
    uint64_t order = phi;
    for (uint64_t p = 2; p * p <= order; ++p) {
        if (order % p == 0) {
            while (order % p == 0 &&
                   modpow10(10, order / p, n) == 1)
                order /= p;
        }
    }
    /* remaining prime factor (if any) is handled automatically */

    /* Midy's theorem holds iff order is even and 10^(order/2) ≡ -1 (mod n) */
    if ((order & 1ULL) != 0) return false;
    return modpow10(10, order / 2, n) == n - 1;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_candidate = 7;   /* first candidate >6 */

    while (cache_len < n) {
        uint64_t cand = (uint64_t)next_candidate;
        if (good_candidate(cand)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

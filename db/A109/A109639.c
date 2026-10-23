/*
 * A109639  Numbers n such that the string 66n is prime.
 * https://oeis.org/A109639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer n such that the decimal * concatenation
 *            66...n is a prime. For example, * n = 1 gives 661, n = 91 gives
 *            6691
 * Range:     u8 n <= 21, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }

    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* bases for deterministic test up to 2^64 */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a;
        uint64_t cur = d;
        while (cur) {
            if (cur & 1ULL) x = (x * p) % n;
            p = (p * p) % n;
            cur >>= 1;
        }
        uint64_t val = (uint64_t)x;
        if (val == 1 || val == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            val = (uint64_t)((( __uint128_t )val * val) % n);
            if (val == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

static inline uint64_t digit_count(uint64_t x)
{
    uint64_t c = 0;
    do {
        ++c;
        x /= 10;
    } while (x);
    return c;
}

static inline uint64_t pow10_uint64(uint64_t d)
{
    uint64_t p = 1;
    while (d--) p *= 10;
    return p;
}

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static size_t terms_cap = 0;
    static uint64_t last_candidate = 0;

    /* increase storage if needed */
    while (terms_size < n) {
        ++last_candidate;
        uint64_t d = digit_count(last_candidate);
        uint64_t p10 = pow10_uint64(d);
        uint64_t val = 66ULL * p10 + last_candidate;
        if (is_prime_uint64(val)) {
            if (terms_size == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 32;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_size++] = (seq_t)last_candidate;
        }
    }
    return terms[n - 1];
}

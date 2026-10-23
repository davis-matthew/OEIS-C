/*
 * A106700  Numbers k such that 9973*k - 1009 is prime.
 * https://oeis.org/A106700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A106700 – the n‑th integer k such that 9973*k-1009 is prime.
    A search is performed; no closed formula is known.  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* inexpensive 64‑bit modular multiplication using 128‑bit intermediate */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* modular exponentiation */
static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e) {
        if (e & 1U)
            res = mod_mul(res, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1U;
    }
    return res;
}

/* deterministic Miller–Rabin for 64‑bit numbers */
static bool isPrime64(uint64_t n)
{
    if (n < 2U)
        return false;

    /* small‑prime trial division for quick elimination */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
        53ULL, 59ULL, 61ULL, 67ULL, 71ULL, 73ULL, 79ULL,
        83ULL, 89ULL, 97ULL, 101ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0ULL)
            return n == p;
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1U;
        ++s;
    }

    /* bases guaranteeing determinism for 64‑bit */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
        9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool composite = true;
        for (unsigned r = 1U; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(0) is undefined – return 0 for safety */
    if (n == 0U)
        return (seq_t)0U;

    uint64_t count = 0ULL;
    uint64_t k = 1ULL;          /* candidate k */

    while (true) {
        /* compute 9973*k - 1009 safely */
        __uint128_t tmp = (__uint128_t)9973ULL * k - 1009ULL;
        uint64_t candidate = (uint64_t)tmp;   /* fits in uint64_t for relevant n */

        if (candidate >= 2ULL && isPrime64(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}

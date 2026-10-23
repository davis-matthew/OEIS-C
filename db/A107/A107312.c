/*
 * A107312  Primes p such that p + 2 and p^2 + 2^2 are primes.
 * https://oeis.org/A107312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mulmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: there is no simple closed form; a(n) is the n‑th prime p such that
 * p, p+2, and p²+4 are all prime.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mulmod64.h"

/* 64‑bit modular multiplication */

/* 64‑bit modular exponentiation */
static inline uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1)
            r = mulmod64(r, a, mod);
        a = mulmod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool isPrime64(uint64_t n)
{
    if (n < 2)
        return false;
    /* Small prime trial division */
    static const uint64_t small[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t p = small[i];
        if (n % p == 0)
            return n == p;
    }

    /* Write n−1 = d · 2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Test set of witnesses – works for all 64‑bit integers */
    static const uint64_t witnesses[] = {
        2, 325, 9375, 28178, 450775, 9780504, 1795265022
    };
    for (size_t wi = 0; wi < sizeof witnesses / sizeof *witnesses; ++wi) {
        uint64_t a = witnesses[wi] % n;
        if (a == 0)
            continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        unsigned r;
        for (r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1)
                break;
        }
        if (r == s)
            return false;          /* composite */
    }
    return true;                   /* prime */
}

/* Exported function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0 */

    uint64_t count = 0;
    uint64_t p = 3;                /* start from first odd prime */

    while (true) {
        if (isPrime64(p)) {
            if (p > 2 && isPrime64(p + 2ULL)) {
                uint64_t pp = p * p + 4ULL;     /* p² + 4 */
                if (isPrime64(pp)) {
                    ++count;
                    if (count == n)
                        return (seq_t)p;
                }
            }
        }
        p += 2ULL;                  /* skip even numbers */
    }
}

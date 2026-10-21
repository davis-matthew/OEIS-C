/*
 * A077318  Sum of terms in n-th row of A077316.
 * https://oeis.org/A077318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the smallest n primes congruent to 1 (mod n)
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* modular multiplication using 128‑bit intermediate (works for 64‑bit modulus) */
static uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* modular exponentiation: base^exp mod mod */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = mulmod64(result, b, mod);
        b = mulmod64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller–Rabin primality test for 64‑bit integers */
static bool isPrime64(uint64_t n) {
    if (n < 2)
        return false;
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        uint64_t p = small[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool good = false;
        for (int r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) {
                good = true;
                break;
            }
        }
        if (!good)
            return false;
    }
    return true;
}

/* Sequence function: sum of the smallest n primes ≡ 1 (mod n) */
seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t found = 0;
    idx_t k = 1;
    while (found < n) {
        uint64_t cand = n * k + 1;
        if (isPrime64(cand)) {
            sum += (seq_t)cand;
            ++found;
        }
        ++k;
    }
    return sum;
}

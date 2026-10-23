/*
 * A201604  Primes of the form 7n^2 + 3.
 * https://oeis.org/A201604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 964, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   mul_mod, pow_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201604: primes of the form 7*n^2 + 3. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mul_mod.h"
#include "../utils/pow_mod.h"

/* 64-bit deterministic Miller–Rabin primality test.  */

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool continue_outer = false;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                continue_outer = true;
                break;
            }
        }
        if (!continue_outer) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */
    idx_t count = 0;
    idx_t k = 0;
    while (true) {
        unsigned __int128 kk = (unsigned __int128)k * k;
        unsigned __int128 val128 = kk * 7 + 3;
        uint64_t val64 = (uint64_t)val128;  /* val128 < 2^64 for the requested n */
        if (is_prime_uint64(val64)) {
            ++count;
            if (count == n) {
                return (seq_t)val64;
            }
        }
        ++k;
    }
}

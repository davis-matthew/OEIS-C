/*
 * A201037  Primes of the form 10n^3-3.
 * https://oeis.org/A201037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 10 k^3 - 3 (k >= 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mulmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mulmod64.h"

static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e != 0) {
        if (e & 1)
            res = mulmod64(res, a, mod);
        a = mulmod64(a, a, mod);
        e >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0)
            return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0)
            continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
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
    /* n is 1‑based index, count primes of form 10*k^3 - 3 */
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        /* Skip k divisible by 3 → number divisible by 3 */
        if ((k % 3) == 0)
            continue;
        /* Compute 10*k^3 - 3 in 64‑bit; safe for k up to ~10^4 */
        uint64_t kk = (uint64_t)k;
        uint64_t val = 10ULL * kk * kk * kk - 3ULL;
        if (is_prime_uint64(val)) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
    }
    /* Should never be reached for valid n */
    return (seq_t)0;
}

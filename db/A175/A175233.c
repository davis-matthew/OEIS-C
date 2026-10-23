/*
 * A175233  Numbers n with property that n and 4*n^2 + 163 are terms in
 *          A057605.
 * https://oeis.org/A175233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer such that 
 * m = 4*n^2 + 163 and p = 4*m^2 + 163 are both prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* modular multiplication (a*b)%mod using 128‑bit arithmetic */
static inline uint64_t _mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* modular exponentiation (base^exp)%mod */
static inline uint64_t _mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = _mod_mul(result, base, mod);
        base = _mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller–Rabin primality test for 64‑bit integers */
static inline bool _is_prime_ull(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0)
            return n == small_primes[i];
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t a = small_primes[i];
        if (a >= n) break;
        uint64_t x = _mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = _mod_mul(x, x, n);
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

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t candidate = 2;

    while (true) {
        uint64_t c = (uint64_t)candidate;
        uint64_t m = 4ULL * c * c + 163ULL;
        if (!_is_prime_ull(m)) {
            ++candidate;
            continue;
        }

        uint64_t p = 4ULL * m * m + 163ULL;
        if (!_is_prime_ull(p)) {
            ++candidate;
            continue;
        }

        ++found;
        if (found == n)
            return (seq_t)c;

        ++candidate;
    }
}

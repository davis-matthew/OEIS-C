/*
 * A095277  Numbers k such that 4*k + 3 is composite.
 * https://oeis.org/A095277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k such that 4*k + 3 is composite
 * Range:     u8 n <= 168, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 3, a(2) = 6, a(3) = 8, …                                    */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Fast modular multiplication using 128‑bit intermediate.          */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* Fast modular exponentiation.                                       */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mul_mod_uint64(result, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit unsigned ints. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* quick check with small primes */
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    size_t i;
    for (i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* bases sufficient for all n < 2^64 */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;          /* skip if base divides n */
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        int r;
        for (r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence generation. */
seq_t a(idx_t n)
{
    /* a(0) is undefined, return 0 for safety */
    if (n == 0) return (seq_t)0;

    uint64_t target = (uint64_t)n;     /* how many composite terms we need */
    uint64_t k = 0;                    /* current candidate k */
    uint64_t found = 0;                /* how many composite terms found */

    while (found < target) {
        ++k;
        uint64_t m = 4 * k + 3;
        if (!is_prime_uint64(m)) {
            ++found;
        }
    }
    return (seq_t)k;
}

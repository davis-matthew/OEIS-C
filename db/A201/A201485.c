/*
 * A201485  Primes of the form 5n^2 + 7.
 * https://oeis.org/A201485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 5k² + 7 (k even, starting with
 *            k = 0)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 887, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* --- Miller–Rabin helpers for 64‑bit values --------------------------------- */
static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp > 0) {
        if (exp & 1U)
            result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1U;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* small primes */
    const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* write n-1 as d * 2^s  with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --------------------------------------------------------------------------- */
/* The main sequence function                                                */
/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) = 7, offset is 1.  Behaviour for n == 0 is unused. */
    if (n == 0)
        return 0;

    idx_t count = 0;
    idx_t k = 0;          /* start with k = 0 → value = 7 */

    while (true) {
        /* value = 5 * k^2 + 7 */
        seq_t val = ((seq_t)5 * (seq_t)k * (seq_t)k) + 7;

        /* all values produced by 5k²+7 are < 2⁶⁴ for the n needed (≤10 000),
           so we can test with 64‑bit Miller–Rabin safely. */
        if (is_prime_uint64((uint64_t)val)) {
            ++count;
            if (count == n)
                return val;
        }

        k += 2; /* odd k give even numbers > 2 → skip them */
    }
}

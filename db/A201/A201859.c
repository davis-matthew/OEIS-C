/*
 * A201859  Primes of the form 8n^2 - 9.
 * https://oeis.org/A201859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 2868, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 51 terms through n = 4655 (2026-11-01)
 * Helpers:   mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th prime of the form 8k^2‑9. */

/* Keyword: nonn, easy
   Offset 1: a(1) = 23, a(2) = 191, … */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

/* compute 8*k^2 - 9 (value fits in 64 bits for n up to 10000) */
static seq_t prime_of_form(idx_t k)
{
    unsigned __int128 t = (unsigned __int128)k * k * 8;
    t -= 9;
    return (seq_t)(uint64_t)t;
}

/* --- 64‑bit Miller–Rabin primality test --------------------------------- */

/* deterministic Miller–Rabin for 64‑bit n */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small prime check */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0)
            return n == small[i];
    }
    /* write n-1 as d * 2^s (d odd) */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t witnesses[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0)
            continue;
        uint64_t x = powmod_u64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
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

/* wrapper for arbitrary seq_t */
static bool is_prime_seq(seq_t x)
{
    return is_prime_uint64((uint64_t)x);
}

/* OEIS sequence A201859: primes of the form 8k^2 - 9 */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 2; ; ++k) {
        seq_t val = prime_of_form(k);
        if (is_prime_seq(val)) {
            ++count;
            if (count == n)
                return val;
        }
    }
}

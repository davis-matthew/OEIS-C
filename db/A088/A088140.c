/*
 * A088140  Duplicate of A005451 (for n >= 3).
 * https://oeis.org/A088140
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 if n is an odd prime, otherwise a(n) = n
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   mul_mod128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 2 → a(2) = 2, a(3) = 1, a(4) = 4, … as in OEIS A088140) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mul_mod128.h"

/* 64‑bit modular exponentiation (binary exponentiation) */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1ULL;
    while (exp > 0) {
        if (exp & 1ULL)
            result = mul_mod128(result, base, mod);
        base = mul_mod128(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit unsigned integers */
static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0ULL) return false;
    }

    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod128(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* sequence function required by OEIS build */
seq_t a(idx_t n)
{
    /* undefined behaviour for n < 2; return 0 for safety */
    if (n < 2) return (seq_t)0;

    /* even numbers (including 2) are always returned as themselves */
    if ((n & 1ULL) == 0ULL)
        return (seq_t)n;

    /* odd numbers */
    if (is_prime64((uint64_t)n))
        return (seq_t)1;  /* odd prime */
    else
        return (seq_t)n;  /* odd composite */
}

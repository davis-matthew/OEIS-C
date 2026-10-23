/*
 * A106124  Primes with maximal digit > 7.
 * https://oeis.org/A106124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   has_digit_8_or_9, mul64_mod, pow64_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime that has at least one decimal digit ≥ 8
   (primes whose maximal digit is > 7, OEIS A106124).
   Implementation details:
   – Uses a deterministic Miller–Rabin test for 64‑bit values.
   – Counts primes in increasing order until the desired rank is reached.
   – Should work for any integer type chosen for seq_t (8, 16, 32, 64
     or 128 bits) as long as the needed values fit in 64 bits,
     which holds for n up to at least 10 000. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/has_digit_8_or_9.h"
#include "../utils/mul64_mod.h"
#include "../utils/pow64_mod.h"

/* Multiply a and b modulo mod using 128‑bit arithmetic (sufficient
   for 64‑bit operands). */

/* Deterministic Miller–Rabin primality test for 64‑bit unsigned integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* Small primes to filter out multiples quickly. */
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* Write n‑1 as d·2^s with d odd. */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* Bases sufficient for deterministic test on 64‑bit integers. */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0)
            continue;
        uint64_t x = pow64_mod(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul64_mod(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Sequence is 1‑based; behaviour for n < 1 is undefined. */
    seq_t count = 0;
    seq_t val = 2;          /* first prime candidate */
    while (count < n)
    {
        /* Cast to 64‑bit for primality testing; the required values
           for n ≤ 10 000 fit comfortably in uint64_t. */
        uint64_t u = (uint64_t)val;
        if (is_prime_uint64(u) && has_digit_8_or_9(u))
        {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
    /* Unreachable, but required by the compiler. */
    return val;
}

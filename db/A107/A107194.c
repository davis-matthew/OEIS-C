/*
 * A107194  Primes of the form 2x^2 + 41y^2.
 * https://oeis.org/A107194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 794, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 2x^2 + 41y^2 */

/* Using the provided primality test from the utils package. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Integer square root (floor) for uint64_t.  This implementation is
   self-contained and does not rely on external libraries. */
static uint64_t int_sqrt_uint64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit)
    {
        if (x >= r + bit)
        {
            x -= r + bit;
            r = (r >> 1) + bit;
        }
        else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* Test whether a prime p can be written as 2*x^2 + 41*y^2. */
static bool repr_2x2_41(uint64_t p)
{
    uint64_t max_y = int_sqrt_uint64(p / 41);
    for (uint64_t y = 0; y <= max_y; ++y)
    {
        unsigned __int128 y2   = (unsigned __int128)y * y;
        unsigned __int128 prod = 41 * y2;
        if (prod > p) break;
        unsigned __int128 rem = (unsigned __int128)p - prod;
        if (rem == 0) /* p == 41*y^2, valid when p is prime (=> y==1) */
            return true;
        if (rem < 2) continue;
        if ((rem & 1) == 0)
        {
            uint64_t t = (uint64_t)(rem >> 1); /* t = x^2 */
            uint64_t s = int_sqrt_uint64(t);
            if (s * s == t)
                return true;
        }
    }
    return false;
}

/* Return the n-th prime of the form 2x^2 + 41y^2 (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static idx_t size   = 0;
    static idx_t cap    = 0;

    /* next_candidate holds the next value to test (starts at 2). */
    static uint64_t next_candidate = 2;

    while (size < n)
    {
        uint64_t candidate = next_candidate;

        /* Advance to the next odd after 2. */
        if (candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;

        /* Test primality. */
        if (!is_prime(candidate))
            continue;

        /* Test representation. */
        if (!repr_2x2_41(candidate))
            continue;

        /* Store the prime. */
        if (size == cap)
        {
            cap = cap ? cap * 2 : 16;
            terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
        }
        terms[size++] = (seq_t)candidate;
    }

    return terms[n - 1];
}

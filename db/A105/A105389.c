/*
 * A105389  Primes of the form x^2 + 32 y^2, also primes p with h(-p)
 *          divisible by 8.
 * https://oeis.org/A105389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 803, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 46 terms through n = 9779 (2026-11-03)
 * Helpers:   is_prime_u64, is_square_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105389: primes of the form x^2 + 32 y^2 */

/* The algorithm generates primes in increasing order and keeps only
   those that can be written as p = x^2 + 32 y^2.  For each prime
   p we test all y such that 32*y^2 <= p; if p - 32*y^2 is a perfect
   square, a representation exists.  The sequence is stored in a
   dynamic array so that calls to a(n) in arbitrary order are
   supported efficiently. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/is_square_u64.h"
#include "../utils/mem.h"

static bool has_representation(uint64_t p)
{
    /* Find integer y with 32*y^2 <= p.  For each such y, check if
       p - 32*y^2 is a perfect square. */
    uint64_t max_y = (uint64_t)sqrt((long double)p / 32.0L);
    for (uint64_t y = 0; y <= max_y; ++y)
    {
        unsigned __int128 prod = (unsigned __int128)32 * y * y;
        uint64_t rem = (uint64_t)(p - (uint64_t)prod);
        if (is_square_u64(rem))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static storage for the computed sequence terms */
    static seq_t *terms = NULL;
    static idx_t  size   = 0;
    static idx_t  cap    = 0;
    static uint64_t candidate = 3;   /* next odd number to examine */

    /* Grow the array when needed */
    if (cap == 0)
    {
        cap = 64;
        terms = mem_new(seq_t, cap);
    }

    /* Generate terms until we have at least n elements */
    while (size < n)
    {
        if (is_prime_u64(candidate) && has_representation(candidate))
        {
            if (size == cap)
            {
                cap <<= 1;
                terms = mem_resize(seq_t, terms, cap);
            }
            terms[size++] = (seq_t)candidate;
        }
        candidate += 2;  /* only odd candidates */
    }

    return terms[n - 1];
}

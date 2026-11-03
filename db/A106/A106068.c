/*
 * A106068  Primes p such that 3p + 4 and 4p + 3 are primes.
 * https://oeis.org/A106068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 400, u32 n <= 534, u64 n <= 534,
 *            u128 n <= 534, big any n
 * Validated: standard, pass, 64 terms through n = 532 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the OEIS sequence A106068: 
   primes p such that 3p+4 and 4p+3 are also primes.
   The algorithm repeatedly tests consecutive integers for primality,
   and keeps those that satisfy the condition.  Results are cached in
   a static dynamic array so that calls may be arbitrary order and
   multiple requests reuse the work already done.
*/
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Cached list of special primes */
    static seq_t *special = NULL;
    static size_t special_cap  = 0;
    static size_t special_sz   = 0;

    /* Current integer to test */
    static uint64_t current = 2;

    /* Ensure we have at least n terms */
    while (special_sz < n)
    {
        /* Skip non‑primes */
        if (is_prime_u64(current))
        {
            uint64_t q = 3 * current + 4;
            uint64_t r = 4 * current + 3;
            if (is_prime_u64(q) && is_prime_u64(r))
            {
                /* Grow storage if needed */
                if (special_sz == special_cap)
                {
                    special_cap = special_cap ? special_cap * 2 : 16;
                    special = mem_resize(seq_t, special, special_cap);
                }
                /* Store the term */
                special[special_sz++] = (seq_t)current;
            }
        }
        /* Advance to next candidate */
        current++;
    }

    /* Return the requested term (index is 1‑based) */
    return special[n - 1];
}

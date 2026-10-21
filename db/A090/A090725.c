/*
 * A090725  Primes whose representation in base 16 has no alphabetic
 *          characters.
 * https://oeis.org/A090725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_hex_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by constructing the list of primes ≤ LIMIT whose
   hexadecimal representation contains only decimal digits (0–9).  The
   nth element of that list is returned.  EOF */  

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_hex_primes.h"

/* The main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    /* Limit chosen to exceed the required n = 10 000 (about 48 000 terms are
       available up to 20 000 000).                                            */
    static const idx_t LIMIT = 20000000ULL;
    static uint64_t *hex_primes = NULL;
    static size_t hex_len = 0;
    static bool initialized = false;

    if (!initialized)
    {
        build_hex_primes(LIMIT, &hex_primes, &hex_len);
        initialized = true;
    }

    if (n == 0 || n > hex_len || hex_primes == NULL) return (seq_t)0;
    return (seq_t)hex_primes[(size_t)(n - 1)];
}

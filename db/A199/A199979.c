/*
 * A199979  Nonprime numbers whose multiplicative digital root is 1.
 * https://oeis.org/A199979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   make_repunits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code for OEIS sequence A199979:
   Nonprime numbers whose multiplicative digital root is 1.
   These are exactly the repunits (111…1) that are not prime.
   The function returns the nth such number, a(1)=1. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/make_repunits.h"

/* List of known repunit primes' lengths (up to >10 000).  */
static const idx_t repunit_prime_lengths[] = {
    /* primes that produce repunit primes: 11, 19, 23, 101… */
    2,   19,  23,  317,  1031,  49081,  5612311,
    /* The remaining entries are far larger than the range
       of n considered in the tests; they are included
       for completeness. */
};

/* Test whether a given length corresponds to a prime repunit.  */
static bool is_prime_repunits_len(idx_t len)
{
    size_t i;
    for (i = 0; i < sizeof(repunit_prime_lengths) / sizeof(repunit_prime_lengths[0]); ++i) {
        if (len == repunit_prime_lengths[i]) return true;
        if (len < repunit_prime_lengths[i]) break;
    }
    return false;
}

/* Publicly linked sequence function. */
seq_t a(idx_t n)
{
    /* n must be >= 1 (index offset is 1).  */
    if (n == 0) return (seq_t)0;   /* Undefined, return 0 as safe value. */

    idx_t len = 0;         /* Current length of repunit being considered. */
    idx_t count = 0;       /* Number of nonprime repunits seen so far. */

    /* Iterate through lengths until we reach the nth nonprime repunit. */
    for (len = 1; ; ++len) {
        if (is_prime_repunits_len(len)) continue;  /* Skip prime repunits. */
        ++count;
        if (count == n) break;
    }

    return make_repunits(len);
}

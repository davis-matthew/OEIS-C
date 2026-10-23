/*
 * A104154  For each natural number n: if the last digit of n is not zero and
 *          A004086(n) is prime, append A004086(n) to the sequence.
 * https://oeis.org/A104154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1229, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Helpers:   is_prime, rev_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104154: for each natural integer n, if n does not end in 0
 * and the reversal of n is prime, the reversed number is appended to the
 * sequence.  a(n) is the nth such reversed prime, indexed from 1. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/rev_uint64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;
    seq_t result = 0;

    for (;; ++m)
    {
        if (m % 10 == 0)         /* skip numbers ending in 0 */
            continue;

        seq_t rev = rev_uint64(m);          /* reversed number as seq_t        */
        uint64_t rev64 = (uint64_t)rev;     /* cast to uint64_t for primality  */

        if (!is_prime(rev64))
            continue;

        ++count;
        if (count == n)
        {
            result = rev;
            break;
        }
    }
    return result;
}

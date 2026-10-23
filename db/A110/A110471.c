/*
 * A110471  Prime analog of Baum-Sweet sequence: a(n) = 1 if binary
 *          representation of n contains no block of consecutive zeros of
 *          exactly prime length; otherwise a(n) = 0.
 * https://oeis.org/A110471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   is_prime_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A110471: a(n) = 1 if the binary representation of n
   contains no maximal block of consecutive zeros of prime length,
   otherwise 0. */
#include <stdbool.h>
#include "../utils/is_prime_len.h"

seq_t a(idx_t n)
{
    /* Special case: n = 0 has the binary representation "0",
       which is considered to contain no prime-length zero run. */
    if (n == 0)
        return (seq_t)1;

    idx_t x   = n;
    int  zeros = 0;

    while (x > 0)
    {
        /* Check the least‑significant bit. */
        if (x & 1)
        {
            /* A zero run has just finished (if any). */
            if (zeros > 0)
            {
                if (is_prime_len(zeros))
                    return (seq_t)0;
                zeros = 0;
            }
        }
        else
        {
            /* We are inside a zero run. */
            ++zeros;
        }
        x >>= 1;
    }

    /* All runs inspected; no prime-length run found. */
    return (seq_t)1;
}

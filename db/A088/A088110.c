/*
 * A088110  Numbers n such that 2^n + n and 2^n - n are noncomposite.
 * https://oeis.org/A088110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th integer m (starting with m = 0) such that 
   both 2^m + m and 2^m – m are either prime or equal to 1. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) corresponds to m = 0 */
    if (n == 0) {
        return (seq_t)0;      /* undefined for n = 0, return 0 safely */
    }

    idx_t  count = 0;          /* how many qualifying m's have been found */
    idx_t  m     = 0;          /* current exponent being tested */
    seq_t  pow2  = (seq_t)1;   /* 2^m */

    while (1) {
        seq_t sum = pow2 + (seq_t)m;
        seq_t diff = pow2 - (seq_t)m;

        /* A number is considered “non‑composite” if it is 1 or prime. */
        bool ok = ((diff == (seq_t)1) || is_prime_big(diff)) &&
                  ((sum  == (seq_t)1) || is_prime_big(sum));

        if (ok) {
            ++count;
            if (count == n) {
                return (seq_t)m;   /* return the n‑th qualifying m */
            }
        }

        /* Prepare for the next exponent.  Detect overflow only for
           unsigned integer types (sizeof(seq_t) ≤ 8).  For arbitrary
           precision types we assume no overflow occurs. */
        ++m;
        if (sizeof(seq_t) <= 8) {
            /* (~(seq_t)0) is the type's maximum value.  If pow2 is larger
               than half that value, multiplying by 2 would overflow. */
            if (pow2 > ((~(seq_t)0) >> 1)) {
                break;          /* cannot test larger exponents safely */
            }
        }
        pow2 = pow2 * (seq_t)2;        /* advance to 2^(m+1) */
    }

    /* If the loop terminates without finding the n‑th term, the
       sequence is exhausted for the current build.  Return 0 as a
       sentinel. */
    return (seq_t)0;
}

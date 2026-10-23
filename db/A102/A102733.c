/*
 * A102733  Numbers n such that 2*n + 101 is prime.
 * https://oeis.org/A102733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th integer k such that 2*k + 101 is prime. */
#include <stdint.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    idx_t count = 0;     /* how many qualifying numbers found */
    idx_t k = 0;         /* current candidate integer */

    /* Keep looking until we reach the nth qualifying number. */
    while (count < n)
    {
        /* 2*k + 101 fits in uint64_t for all practical k used here.  */
        seq_t p = (seq_t)((uint64_t)2 * k + (uint64_t)101);
        if (is_prime_seq(p))
        {
            ++count;
            if (count == n)
                return (seq_t)k;   /* cast the int to seq_t */
        }
        ++k;
    }

    /* Unreachable: the loop always returns.  */
    return (seq_t)0;
}

/*
 * A098117  a(n) = 5^(prime(n) - 1) + 5^prime(n) - 1.
 * https://oeis.org/A098117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(prime(n)-1) + 5^prime(n) - 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   get_prime, pow5_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow5_seq.h"

/* External function that returns the nth term of A098117. */
seq_t a(idx_t n)
{
    /* Suppose that idx_t is 1‑based, not 0‑based. */
    idx_t p = get_prime(n);                 /* p = prime(n)             */
    seq_t p1 = pow5_seq((seq_t)(p - 1));    /* 5^(p-1)                   */
    seq_t p2 = pow5_seq((seq_t)p);          /* 5^p                       */
    return p1 + p2 - (seq_t)1;              /* 5^(p-1)+5^p - 1           */
}

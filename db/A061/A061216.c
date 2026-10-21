/*
 * A061216  a(n) = product of all even numbers between n-th prime and (n+1)-st
 *          prime.
 * https://oeis.org/A061216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all even numbers strictly between p_n and
 *            p_{n+1}
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 98, u128 n <= 216,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A061216) */
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* handle n >= 1; a(1) = 1 by the loop below */
    uint32_t p1 = get_prime(n);   /* nth prime */
    uint32_t p2 = get_prime(n + 1); /* (n+1)-th prime */

    seq_t prod = 1;

    /* First even integer greater than p1 */
    idx_t start = (idx_t)p1 + 1;
    if ((start & 1) != 0)
        start += 1;          /* make even */

    /* Last integer less than p2 */
    idx_t end = (idx_t)p2 - 1;

    /* Multiply all even numbers in [start, end] */
    for (idx_t v = start; v <= end; v += 2) {
        prod *= (seq_t)v;
    }

    return prod;
}

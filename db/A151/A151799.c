/*
 * A151799  Version 2 of the "previous prime" function: largest prime < n.
 * https://oeis.org/A151799
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = largest prime < n (previous prime function)
 * Range:     u8 n <= 257, u16 n <= 9947, u32 n <= 9947, u64 n <= 9947,
 *            u128 n <= 9947, big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 3  (a(3) = 2) */

#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;          /* behaviour below offset is irrelevant */
    return (seq_t)prev_prime(n);
}

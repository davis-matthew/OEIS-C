/*
 * A122601  a(n)=(n-th prime +1) modulo 7.
 * https://oeis.org/A122601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (get_prime(n)+1) mod 7
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the n-th prime (1‑based index) */
    uint32_t p = get_prime(n);           /* p fits in 32 bits for the tested range */

    /* compute (p + 1) modulo 7 in seq_t type */
    seq_t r = (seq_t)p + 1;
    r %= 7;

    return r;
}

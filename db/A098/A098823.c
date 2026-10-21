/*
 * A098823  a(n) = 16*(8*prime(n) + 7).
 * https://oeis.org/A098823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 * (8 * prime(n) + 7)
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A098823. */
#include <stdint.h>                  /* for uint32_t */
#include "../utils/get_prime.h"      /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; below is undefined */
    if (n < 1)
        return 0;

    /* obtain the n-th prime (1‑based) */
    uint32_t p = get_prime(n);

    /* perform the computation in the seq_t type */
    seq_t tmp = (seq_t)8 * (seq_t)p + (seq_t)7;
    return (seq_t)16 * tmp;
}

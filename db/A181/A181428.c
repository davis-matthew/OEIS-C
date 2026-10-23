/*
 * A181428  a(n) = prime(n+2) + prime(n+1) - prime(n).
 * https://oeis.org/A181428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+2) + prime(n+1) - prime(n)
 * Range:     u8 n <= 52, u16 n <= 6538, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is 1‑based; get_prime(k) returns the k‑th prime (k≥1) */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    seq_t p1 = (seq_t)get_prime(n);       /* prime(n)   */
    seq_t p2 = (seq_t)get_prime(n + 1);   /* prime(n+1) */
    seq_t p3 = (seq_t)get_prime(n + 2);   /* prime(n+2) */
    return p3 + p2 - p1;
}

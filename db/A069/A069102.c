/*
 * A069102  a(1) = 1; a(2) = 1; a(n) = Prime[n-1] + Prime[n-2] if n > 2.
 * https://oeis.org/A069102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1,2; otherwise * a(n) = Prime[n-1] + Prime[n-2]
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function get_prime from the utils library. */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)1;
    /* get_prime returns the nth prime (1‑based). */
    seq_t p1 = (seq_t)get_prime(n - 1);
    seq_t p2 = (seq_t)get_prime(n - 2);
    return p1 + p2;
}

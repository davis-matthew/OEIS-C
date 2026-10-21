/*
 * A094814  a(n) = n^(prime(prime(n))).
 * https://oeis.org/A094814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(prime(prime(n)))
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n-th prime */
    idx_t p1 = get_prime(n);
    /* prime of that index */
    idx_t p  = get_prime(p1);
    /* compute the power in the big‑int type */
    return seq_pow((seq_t)n, p);
}

/*
 * A097330  In the sequence of prime numbers replace each term p with
 *          floor(p/2) and ceiling(p/2).
 * https://oeis.org/A097330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n < 3, otherwise (prime(((n)+(n mod 2))/2) + 1)/2 -
 *            (n mod 2)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The first three terms are all 1. */
    if (n < 3u)
        return 1;

    /* k = ceil(n/2) = (n+1)/2 gives the index of the prime to use. */
    idx_t k = (n + 1u) >> 1;               /* equivalent to (n + 1) / 2 */

    /* Fetch the k-th prime (uint32_t is sufficient). */
    uint32_t pk = get_prime(k);

    /* Compute floor(pk/2) and ceil(pk/2). */
    seq_t floor_half = (seq_t)(pk >> 1);                 /* pk / 2 */
    seq_t ceil_half  = (seq_t)((pk + 1u) >> 1);          /* (pk + 1) / 2 */

    /* Return according to parity of n: odd -> floor, even -> ceil. */
    return (n & 1u) ? floor_half : ceil_half;
}

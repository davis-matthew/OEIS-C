/*
 * A174254  p-x^x where p is prime number and x is largest integer such that
 *          x^x is smaller than p.
 * https://oeis.org/A174254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n - maximal x^x with x^x < p_n (p_n denotes the n‑th
 *            prime, 1‑indexed)
 * Range:     u8 n <= 9, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Obtain the n‑th prime.  get_prime is 1‑indexed. */
    seq_t p = (seq_t)get_prime(n);

    idx_t x = 1;               /* current candidate for x   */
    seq_t prev_pow = (seq_t)1; /* 1^1 = 1, guaranteed to be < p for any prime ≥2 */

    while (1)
    {
        seq_t cur_pow = big_pow((seq_t)x, (seq_t)x); /* compute x^x */
        if (cur_pow >= p)                          /* stop when too large   */
            break;
        prev_pow = cur_pow;                        /* keep the last acceptable value */
        ++x;
    }

    return p - prev_pow;
}

/*
 * A130291  Number of quadratic residues (including 0) modulo the n-th prime.
 * https://oeis.org/A130291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(p_n/2) + 1 where p_n is the n‑th prime
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Fetch the n-th prime. The helper returns uint32_t. */
    uint32_t p = get_prime(n);

    /* Integer division truncates towards zero, giving floor(p/2). */
    seq_t half = (seq_t)(p / 2);

    /* Add one to count the residue 0. */
    return half + 1;
}

/*
 * A078318  Sum of divisors of n*rad(n)+1, where rad = A007947 (squarefree
 *          kernel).
 * https://oeis.org/A078318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma_of(n * rad(n) + 1) (sum of divisors of n·rad(n)+1)
 * Range:     u8 n <= 12, u16 n <= 186, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"
#include "../utils/sigma_of.h"

/* Compute the OEIS sequence A078318:
 * a(n) = sum of divisors of n*rad(n)+1
 */
seq_t a(idx_t n)
{
    /* Compute the square‑free kernel (product of distinct primes) */
    seq_t rad_n = rad(n);

    /* Compute m = n * rad(n) + 1   (use seq_t arithmetic to avoid overflow) */
    seq_t m_seq = (seq_t)n * rad_n + 1;

    /* m fits in idx_t because n*rad(n) ≤ n² and n ≤ 10000 → m ≤ 1e8 */
    idx_t m = (idx_t)m_seq;

    /* Return the sum of divisors of m */
    return sigma_of(m);
}

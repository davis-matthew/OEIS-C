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
 * Validated: standard, pass, 66 terms through n = 9476 (2026-11-01)
 * Helpers:   radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/radical.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Compute the OEIS sequence A078318:
 * a(n) = sum of divisors of n*rad(n)+1
 */
seq_t a(idx_t n)
{
    /* Compute the square‑free kernel (product of distinct primes) */
    seq_t rad_n = radical(n);

    /* Compute m = n * rad(n) + 1   (use seq_t arithmetic to avoid overflow) */
    seq_t m_seq = (seq_t)n * rad_n + 1;

    /* m fits in idx_t because n*rad(n) ≤ n² and n ≤ 10000 → m ≤ 1e8 */
    idx_t m = (idx_t)m_seq;

    /* Return the sum of divisors of m */
    return sigma_of(m);
}

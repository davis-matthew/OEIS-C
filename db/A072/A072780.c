/*
 * A072780  a(n) = sigma_2(n) + phi(n) * sigma(n) - 2*n^2, which is A072779(n)
 *          - 2*n^2.
 * https://oeis.org/A072780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = sigma_2(n) + phi(n) * sigma(n) - 2*n^2
 * Range:     u8 n <= 31, u16 n <= 431, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi, sigma_k, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A072780 */

#include "../utils/sigma_k.h"
#include "../utils/sigma_of.h"
#include "../utils/phi.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* sigma_2(n) */
    seq_t sigma2 = sigma_k(n, 2);

    /* sigma(n) – sum of divisors */
    seq_t sigma_n = sigma_of(n);

    /* phi(n) – Euler's totient */
    seq_t phi_n = phi(n);

    /* 2 * n^2 calculated in seq_t to avoid overflow. */
    seq_t n_sq   = (seq_t)n * (seq_t)n;
    seq_t two_n_sq = n_sq * 2;

    /* a(n) formula */
    return sigma2 + phi_n * sigma_n - two_n_sq;
}

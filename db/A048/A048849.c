/*
 * A048849  a(n) = prime(phi(n)) + phi(prime(n)).
 * https://oeis.org/A048849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(phi(n)) + phi(prime(n))
 * Range:     u8 n <= 36, u16 n <= 3516, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Compute prime(phi(n)) */
    seq_t phi_n = calc_phi((seq_t)n);
    idx_t phi_idx = (idx_t)phi_n;              /* phi(n) is guaranteed to fit in idx_t */
    seq_t prime_phi_n = (seq_t)get_prime(phi_idx);

    /* Compute phi(prime(n)) */
    seq_t prime_n = (seq_t)get_prime(n);
    seq_t phi_prime_n = calc_phi(prime_n);

    return prime_phi_n + phi_prime_n;
}

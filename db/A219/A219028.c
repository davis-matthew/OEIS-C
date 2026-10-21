/*
 * A219028  Number of non-primitive roots for prime(n), less than prime(n).
 * https://oeis.org/A219028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n - 1 - phi(phi(p_n)), where p_n is the n‑th prime
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/calc_phi.h"

/* Compute a(n) for any n ≥ 1.  For n < 1 a zero is returned as a sentinel. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) {
        return (seq_t)0;
    }

    /* Retrieve the n‑th prime as an unsigned 32‑bit value. */
    uint32_t prime32 = get_prime(n);

    /* Convert to seq_t for further manipulation. */
    seq_t p   = (seq_t)prime32;
    seq_t phi1 = calc_phi(p);
    seq_t phi2 = calc_phi(phi1);

    return p - (seq_t)1 - phi2;
}

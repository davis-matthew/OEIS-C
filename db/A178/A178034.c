/*
 * A178034  a(n) = binomial(n*Omega(n),Omega(n)) / n.
 * https://oeis.org/A178034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n*Omega(n),Omega(n)) / n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 47, u64 n <= 255,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Omega(n) is the number of prime factors of n counted with multiplicity */

#include <stddef.h>
#include <stdint.h>

/* External utilities that may be used */
#include "../utils/bigomega.h"
#include "../utils/binomial.h"

/* Return the n-th term of OEIS sequence A178034 */
seq_t a(idx_t n)
{
    /* Compute Ω(n) (bigomega returns a seq_t, but the value is small) */
    seq_t omega_seq = bigomega(n);
    idx_t omega = (idx_t)omega_seq;   /* safe cast: Ω(n) fits in a small integer */

    /* Compute binomial(n * Ω(n), Ω(n)) */
    idx_t N = n * omega;               /* product fits in idx_t for the intended range */
    seq_t bin = binomial(N, omega);

    /* Divide by n (exact division by definition) */
    seq_t result = bin / (seq_t)n;
    return result;
}

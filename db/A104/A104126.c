/*
 * A104126  a(n) = prime(n)^(prime(n)+1).
 * https://oeis.org/A104126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n)^(prime(n)+1) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* get the n-th prime */
    seq_t p = get_prime(n);

    /* exponent is p+1 (fits in 64‑bit for the tested range) */
    idx_t exp = (idx_t)((unsigned long long)p + 1ULL);

    /* compute p^exp */
    return pow_seq(p, exp);
}

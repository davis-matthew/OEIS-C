/*
 * A098220  a(n) = floor(sigma(sigma(p))/p) = floor(sigma(p+1)/p) =
 *          floor(A008333(n)/p), where p is the n-th prime number.
 * https://oeis.org/A098220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sigma(p+1) / p ) where p is the n-th prime
 * Range:     u8 n <= 27, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   get_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; return 0 for invalid indices. */
    uint32_t p = get_prime(n);   /* nth prime (1‑indexed). */
    seq_t sigma_val = sigma_of((idx_t)p + 1);   /* sigma(p+1). */
    return sigma_val / (seq_t)p;  /* floor division. */
}

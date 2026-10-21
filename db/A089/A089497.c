/*
 * A089497  mu(prime(n)+1) - mu(prime(n)-1), where mu is the Moebius function.
 * https://oeis.org/A089497
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = mu(prime(n)+1) - mu(prime(n)-1)
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   get_prime, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0; /* undefined for offset < 2 */

    uint32_t p = get_prime(n);      /* nth prime, 1‑based */
    int mu_plus  = mu_of((idx_t)(p + 1));
    int mu_minus = mu_of((idx_t)(p - 1));

    return (seq_t)mu_plus - (seq_t)mu_minus;
}

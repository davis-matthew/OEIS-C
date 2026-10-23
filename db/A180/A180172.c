/*
 * A180172  a(n) = gcd(prime(n)+2, n).
 * https://oeis.org/A180172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(prime(n)+2, n)
 * Range:     u8 n <= 56, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Guaranteed to be called with n >= 1 as per sequence offset */
    if (n == 0) return 0;  /* undefined for n < 1, safe fallback */

    uint32_t p = get_prime(n);          /* nth prime (as uint32_t)  */
    seq_t p_plus_2 = (seq_t)p + (seq_t)2; /* prime(n)+2 as seq_t   */
    seq_t n_val     = (seq_t)n;           /* n as seq_t            */

    return gcd(p_plus_2, n_val);
}

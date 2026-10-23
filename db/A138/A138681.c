/*
 * A138681  Prime(n)^12 mod prime(n-1).
 * https://oeis.org/A138681
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^12 mod prime(n-1)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        /* a(0) and a(1) are undefined for this sequence; return 0 as a placeholder. */
        return 0;
    }

    /* Retrieve the nth prime and its predecessor. */
    seq_t p_n   = (seq_t)get_prime(n);
    seq_t p_nm1 = (seq_t)get_prime(n - 1);

    /* Compute p_n^12 mod p_nm1.  The exponent 12 is a small integer. */
    return mod_pow(p_n, 12, p_nm1);
}

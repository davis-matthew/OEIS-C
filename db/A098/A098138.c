/*
 * A098138  a(n) = p^n + n^p where p = prime(n).
 * https://oeis.org/A098138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^n + n^prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offsets start at 1 */

    /* nth prime, as a uint32_t, cast to seq_t */
    seq_t p = (seq_t)get_prime(n);

    /* compute p^n and n^p */
    seq_t p_pow_n = big_pow(p, (seq_t)n);
    seq_t n_pow_p = big_pow((seq_t)n, p);

    return p_pow_n + n_pow_p;
}

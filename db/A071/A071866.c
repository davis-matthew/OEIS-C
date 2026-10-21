/*
 * A071866  Number of elements in the continued fraction for
 *          prime(n+1)/prime(n).
 * https://oeis.org/A071866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of elements in the continued fraction of
 *            prime(n+1)/prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cf_length, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/cf_length.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;            /* index below offset; behaviour undefined but harmless */
    }
    uint32_t p_num = get_prime(n + 1);  /* prime(n+1) */
    uint32_t p_den = get_prime(n);      /* prime(n)   */
    return cf_length(p_num, p_den);
}

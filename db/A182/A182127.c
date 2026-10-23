/*
 * A182127  Main diagonal T(n,n) of array T(k,n) = n-th value of m^prime(k) +
 *          m*prime(k) + prime(k)^m.
 * https://oeis.org/A182127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^p_n + (n-1)*p_n + p_n^(n-1) where p_n is the nth
 *            prime and all operations are in seq_t
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t when needed */
    idx_t m_idx = n - 1;                     /* m = n-1 */
    uint32_t p_uint = get_prime(n);          /* p_n as uint32_t */

    seq_t m = (seq_t)m_idx;                  /* (n-1) as seq_t */
    seq_t p = (seq_t)p_uint;                 /* nth prime as seq_t */

    /* Compute the three terms */
    seq_t term1 = big_pow(m, p);             /* (n-1)^p_n   */
    seq_t term2 = m * p;                     /* (n-1)*p_n   */
    seq_t term3 = big_pow(p, m);             /* p_n^(n-1)   */

    /* Sum them to get a(n) */
    return term1 + term2 + term3;
}

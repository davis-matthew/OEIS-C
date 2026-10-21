/*
 * A207480  a(n) = (3/2)*(1+prime(n)) - prime(n+1).
 * https://oeis.org/A207480
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3/2)*(1+prime(n)) - prime(n+1) (for n ≥ 2)
 * Range:     u8 n <= 23, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined for n < 2; not expected to be called */

    /* obtain the nth prime and the (n+1)th prime */
    seq_t p_n    = (seq_t)get_prime(n);
    seq_t p_next = (seq_t)get_prime(n + 1);

    /* compute (3/2)*(p_n + 1) - p_next */
    seq_t t   = p_n + (seq_t)1;          /* p_n + 1  */
    seq_t num = (seq_t)3 * t;            /* 3 * (p_n + 1) */
    seq_t mid = num / (seq_t)2;          /* divide by 2 (exact since p_n is odd for n≥2) */
    seq_t res = mid - p_next;            /* final value */

    return res;
}

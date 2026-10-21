/*
 * A062839  a(n) = floor((prime(n-1)+prime(n+1))/2).
 * https://oeis.org/A062839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((prime(n-1)+prime(n+1))/2)
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p_prev;
    if (n > 1)
        p_prev = (seq_t)get_prime(n - 1);
    else
        p_prev = 0;
    seq_t p_next = (seq_t)get_prime(n + 1);
    return (p_prev + p_next) / 2;
}

/*
 * A100820  Number of odd numbers between prime(n) and prime(n+1).
 * https://oeis.org/A100820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n+1)-prime(n))/2-1 for n≥2; a(1)=0
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t p1 = get_prime(n);
    seq_t p2 = get_prime(n + 1);
    seq_t diff = p2 - p1;
    seq_t half = diff / 2;
    return half - 1;
}

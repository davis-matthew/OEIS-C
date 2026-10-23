/*
 * A136268  Cyclic p-roots of prime lengths p(n).
 * https://oeis.org/A136268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binom(2*prime(n)-2, prime(n)-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binom, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/binom.h"       /* seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint32_t p = get_prime(n);
    idx_t N = ((idx_t)2 * (idx_t)p) - 2;
    idx_t K = (idx_t)p - 1;
    return binom(N, K);
}

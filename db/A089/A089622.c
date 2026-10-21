/*
 * A089622  a(n) = n^n + n-th prime.
 * https://oeis.org/A089622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^n + n-th prime
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   _pow_seq_t, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_t.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t pow_n = _pow_seq_t((seq_t)n, n);
    seq_t prime = (seq_t)get_prime(n);
    return pow_n + prime;
}

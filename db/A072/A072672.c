/*
 * A072672  Prime(n)*prime(2*n)+prime(n)+prime(2*n).
 * https://oeis.org/A072672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)*prime(2*n) + prime(n) + prime(2*n)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* behaviour for n < 1 is irrelevant for the sequence */

    seq_t p1 = (seq_t) get_prime(n);
    seq_t p2 = (seq_t) get_prime(2 * n);

    return p1 * p2 + p1 + p2;
}

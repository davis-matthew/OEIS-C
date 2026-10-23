/*
 * A135484  a(n) = Sum_{i=1..n} i^prime(i), where prime(i) denotes i-th prime
 *          number.
 * https://oeis.org/A135484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} i^prime(i)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t p = (idx_t)get_prime(i);       /* ith prime */
        seq_t term = seq_pow((seq_t)i, p);   /* i^p */
        sum += term;
    }
    return sum;
}

/*
 * A066341  Sum of distinct terms in n-th row of Fermat's triangle.
 * https://oeis.org/A066341
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*2^(omega(n)-1) - n, where omega(n) = A001221(n)
 * Range:     u8 n <= 83, u16 n <= 4619, u32 n <= 9623, u64 n <= 9623,
 *            u128 n <= 9623, big any n
 * Helpers:   count_distinct_primes, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    int omega = count_distinct_primes((uint64_t)n);
    seq_t pow = pow2((idx_t)(omega - 1));
    seq_t n_seq = (seq_t)n;
    return (n_seq + 1) * pow - n_seq;
}

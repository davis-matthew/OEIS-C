/*
 * A132927  Concatenation of first n elements of the divisor function d(n),
 *          where d(n) is the number of divisors of n.
 * https://oeis.org/A132927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat_{k=1..n} d(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   digit_len, divisor_count_u64, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* d(k) is the divisor count of k.
 * The result is built by concatenating the decimal representations of
 * the divisor counts of 1,2,...,n. */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t d = divisor_count_u64((uint64_t)k);
        seq_t d_seq = (seq_t)d;
        int digs = digit_len(d_seq);
        seq_t pow = pow10((idx_t)digs);
        result = result * pow + d_seq;
    }
    return result;
}

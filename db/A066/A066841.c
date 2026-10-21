/*
 * A066841  a(n) = Product{k|n} k^(n/k); product is over the positive divisors
 *          of n.
 * https://oeis.org/A066841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = product of d^(n/d) over all positive divisors d|n (OEIS
 *            A066841)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 17, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow_seq_t_local(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    seq_t result = (seq_t)1;

    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            uint64_t e = N / d;                // second divisor
            seq_t t1 = pow_seq_t_local((seq_t)d, e);
            result = result * t1;
            if (d != e) {                      // distinct pair
                seq_t t2 = pow_seq_t_local((seq_t)e, d);
                result = result * t2;
            }
        }
    }
    return result;
}

/*
 * A161397  a(n) = the smallest positive integer that contains the same number
 *          of (non-leading) 0's as n when a(n) and n are written in binary,
 *          is not coprime to n, and is not a divisor of n.
 * https://oeis.org/A161397
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive integer m such that * zero_count(m) =
 *            zero_count(n), * gcd(m,n) > 1, and n % m != 0 * (zero_count
 *            counts the number of 0 bits in the binary representation * of
 *            the number, excluding the leading 1)
 * Range:     u8 n <= 28, u16 n <= 190, u32 n <= 190, u64 n <= 2038,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, zeros_seq, zeros_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/zeros_seq.h"
#include "../utils/zeros_uint64.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0; /* below offset, undefined */

    int zeros_n = zeros_uint64((uint64_t)n);
    seq_t m = (seq_t)1;

    for (;;) {
        if (zeros_seq(m) == zeros_n) {
            seq_t g = gcd(m, (seq_t)n);
            if (g > 1) {
                seq_t rem = (m <= (seq_t)n) ? ((seq_t)n % m) : ((seq_t)n);
                if (rem != 0)
                    return m;
            }
        }
        ++m;
    }
}

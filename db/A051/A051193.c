/*
 * A051193  a(n) = Sum_{k=1..n} lcm(n,k).
 * https://oeis.org/A051193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(1+Sum_{d|n} d*phi(d))/2 (OEIS A051193)
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 2052, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9965 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_seq.h"

/* Computes a(n) = Sum_{k=1..n} lcm(n,k) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1, safety */

    seq_t sumDiv = (seq_t)0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t phi1 = totient_seq((seq_t)d1);
            sumDiv += (seq_t)d1 * phi1;

            if (d1 != d2) {
                seq_t phi2 = totient_seq((seq_t)d2);
                sumDiv += (seq_t)d2 * phi2;
            }
        }
    }

    seq_t t = sumDiv + (seq_t)1;
    return ((seq_t)n * t) / (seq_t)2;
}

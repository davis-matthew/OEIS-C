/*
 * A126828  Ramanujan numbers (A000594) read mod 81.
 * https://oeis.org/A126828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n^2 + k) * sigma_7(n)) mod 81 * where k = 9 if n % 3 ==
 *            2, otherwise k = 0. * sigma_7(n) is the sum of the 7th powers of
 *            the divisors of n
 * Range:     u8 n <= 560, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma7_mod81
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma7_mod81.h"
#ifndef INCLUDED_SEQUENCES_A126828
#define INCLUDED_SEQUENCES_A126828

seq_t a(idx_t n) {
    /* n is guaranteed >= 1 */
    seq_t n_seq = (seq_t)n;
    seq_t n2_mod81 = (n_seq * n_seq) % 81;
    seq_t k = (n % 3 == 2) ? 9 : 0;
    seq_t sigma = sigma7_mod81(n);
    seq_t prod = (n2_mod81 + k) % 81;
    prod = (prod * sigma) % 81;
    return prod;
}

#endif /* INCLUDED_SEQUENCES_A126828 */

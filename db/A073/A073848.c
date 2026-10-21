/*
 * A073848  Smallest of n consecutive numbers in A.P. with a common difference
 *          of n with a sum that is an n-th power.
 * https://oeis.org/A073848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p1*p2*...*pr)^n / n - n*(n-1)/2 for n ≠ 4, where n =
 *            p1^b1 * … * pr^br. a(4) = 58
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 4) {
        return (seq_t)58;
    }

    /* Compute rad(n): product of distinct prime factors of n */
    uint64_t temp = (uint64_t)n;
    seq_t rad = (seq_t)1;

    if (temp % 2 == 0) {
        rad *= (seq_t)2;
        while (temp % 2 == 0) {
            temp /= 2;
        }
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            rad *= (seq_t)p;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    if (temp > 1) {
        rad *= (seq_t)temp;
    }

    /* Compute (rad^n) / n - n*(n-1)/2 */
    seq_t n_seq = (seq_t)n;
    seq_t pow_val = big_pow(rad, n_seq);
    seq_t subterm = n_seq * (n_seq - (seq_t)1) / (seq_t)2;
    seq_t result = pow_val / n_seq - subterm;

    return result;
}

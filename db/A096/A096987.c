/*
 * A096987  Numerator of Sum_{k=1..n} 1/H(k), where H(k) = Sum_{j=1..k} 1/j is
 *          the k-th harmonic number.
 * https://oeis.org/A096987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of ∑_{k=1..n} 1/H(k), with H(k)=∑_{j=1..k} 1/j
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    seq_t sum_num = (seq_t)0;
    seq_t sum_den = (seq_t)1;

    seq_t H_num = (seq_t)0;
    seq_t H_den = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;

        /* H(k) = H_num/H_den + 1/k = (H_num*k + H_den) / (H_den*k) */
        seq_t new_H_num = H_num * k_seq + H_den;
        seq_t new_H_den = H_den * k_seq;
        seq_t gH = gcd(new_H_num, new_H_den);
        H_num = new_H_num / gH;
        H_den = new_H_den / gH;

        /* term 1/H(k) = H_den / H_num */
        seq_t add_num = H_den;
        seq_t add_den = H_num;
        seq_t gAdd = gcd(add_num, add_den);
        add_num /= gAdd;
        add_den /= gAdd;

        /* sum_num/sum_den + add_num/add_den */
        seq_t gSum = gcd(sum_den, add_den);
        seq_t part1 = add_den / gSum;       /* multiplier for sum_num */
        seq_t part2 = sum_den / gSum;       /* multiplier for add_num  */
        seq_t new_sum_num = sum_num * part1 + add_num * part2;
        seq_t new_sum_den = sum_den * part1;
        seq_t gSum2 = gcd(new_sum_num, new_sum_den);
        sum_num = new_sum_num / gSum2;
        sum_den = new_sum_den / gSum2;
    }
    return sum_num;
}

/*
 * A184976  a(n) = [e]+[2*e]+...+[n*e], where []=floor.
 * https://oeis.org/A184976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(e) + floor(2e) + ... + floor(ne)
 * Range:     u8 n <= 13, u16 n <= 219, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 10^9 denominator and rounded numerator of e * 10^9   */
#define E_DEN 1000000000ULL
#define E_NUM 2718281828ULL

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* arbitrary below offset */

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        /* compute floor(k*E_NUM / E_DEN) using 64‑bit arithmetic */
        uint64_t prod = (uint64_t)k * E_NUM;
        seq_t term = (seq_t)(prod / E_DEN);
        sum += term;
    }
    return sum;
}

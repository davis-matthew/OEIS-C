/*
 * A003714  Fibbinary numbers: if n = F(i1) + F(i2) + ... + F(ik) is the
 *          Zeckendorf representation of n (i.e., write n in Fibonacci number
 *          system) then a(n) = 2^(i1 - 2) + 2^(i2 - 2) + ... + 2^(ik - 2).
 *          Also numbers whose binary representation contains no two adjacent
 *          1's.
 * https://oeis.org/A003714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, look
 * Formula:   Zeckendorf representation n = F(i1) + ... + F(ik) (greedy, no
 *            two consecutive Fibonacci numbers); a(n) = 2^(i1-2) + ... +
 *            2^(ik-2)
 * Range:     u8 n <= 54, u16 n <= 2583, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* f[j] = F(j+2): 1, 2, 3, 5, 8, ...  F(93) is the largest Fibonacci number that fits in 64 bits */
    uint64_t f[92];
    int k;
    int i;
    uint64_t rem = (uint64_t)n;
    seq_t result = 0;

    f[0] = 1;
    f[1] = 2;
    k = 2;
    while (k < 92 && f[k - 1] <= rem) {
        f[k] = f[k - 1] + f[k - 2];
        k++;
    }

    /* Greedy Zeckendorf: f[j] is the coefficient of 2^j in the result */
    for (i = k - 1; i >= 0; i--) {
        result = result * 2;
        if (f[i] <= rem) {
            rem -= f[i];
            result = result + 1;
        }
    }
    return result;
}

/*
 * A193770  Table T(m,n) = (5^m + 3^n)/2, m,n = 0,1,2,..., read by
 *          antidiagonals.
 * https://oeis.org/A193770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (5^m + 3^l)/2 where (m,l) are the antidiagonal
 *            coordinates of index n: k = floor((sqrt(1+8n)-1)/2) // diagonal
 *            number t = k*(k+1)/2 // number of elements before diagonal k
 *            offset = n - t // m = offset l = k - m // n = k - offset
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 133, u64 n <= 433,
 *            u128 n <= 1651, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find diagonal k such that k*(k+1)/2 <= n < (k+1)*(k+2)/2 */
    idx_t k = 0;
    __uint128_t t = 0;
    while (1) {
        idx_t add = k + 1;
        if (t + add > n) break;
        t += add;
        k++;
    }
    idx_t t32 = (idx_t)t;       /* t fits in idx_t because t <= n */
    idx_t offset = n - t32;     /* m = offset */
    idx_t m = offset;
    idx_t l = k - offset;

    seq_t five_power = big_pow((seq_t)5, (seq_t)m);
    seq_t three_power = big_pow((seq_t)3, (seq_t)l);
    seq_t sum = five_power + three_power;
    return sum / (seq_t)2;
}

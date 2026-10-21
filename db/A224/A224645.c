/*
 * A224645  Number of (n+2)X4 0..1 matrices with each 3X3 subblock having the
 *          same population.
 * https://oeis.org/A224645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let N = n + 2 be the number of rows. Partition the rows into
 *            three residue classes modulo 3 with counts cnt[0], cnt[1],
 *            cnt[2]. For each class k define A_k = 2 + 2 * 3^{cnt[k]} B_k = 2
 *            + 2^{cnt[k]} Then the sequence satisfies a(n) = (A_0 * A_1 *
 *            A_2) + 2 * ( A_0 * B_1 * B_2 + A_1 * B_0 * B_2 + A_...
 * Range:     u8 none, u16 n <= 5, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   pow_seq_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_n.h"

seq_t a(idx_t n)
{
    idx_t N = n + 2;                  /* number of rows */
    idx_t cnt0 = N / 3;
    idx_t cnt1 = cnt0 + ((N % 3) >= 1 ? 1 : 0);
    idx_t cnt2 = cnt0 + ((N % 3) >= 2 ? 1 : 0);

    /* A_k = 2 + 2 * 3^{cnt[k]} */
    seq_t A0 = 2 + (seq_t)2 * pow_seq_n((seq_t)3, cnt0);
    seq_t A1 = 2 + (seq_t)2 * pow_seq_n((seq_t)3, cnt1);
    seq_t A2 = 2 + (seq_t)2 * pow_seq_n((seq_t)3, cnt2);

    /* B_k = 2 + 2^{cnt[k]} */
    seq_t B0 = 2 + pow_seq_n((seq_t)2, cnt0);
    seq_t B1 = 2 + pow_seq_n((seq_t)2, cnt1);
    seq_t B2 = 2 + pow_seq_n((seq_t)2, cnt2);

    seq_t prodA = A0 * A1 * A2;

    seq_t prod0 = A0 * B1 * B2;
    seq_t prod1 = A1 * B0 * B2;
    seq_t prod2 = A2 * B0 * B1;

    seq_t sum = prod0 + prod1 + prod2;

    return prodA + (seq_t)2 * sum;
}

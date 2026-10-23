/*
 * A152967  Partial sums of A152770.
 * https://oeis.org/A152967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} (i-1)*floor((n-i)/i)
 * Range:     u8 n <= 33, u16 n <= 459, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;               /* a(1)=0, an entry below offset irrelevant */

    seq_t sumA = 0;
    seq_t sumB = 0;
    idx_t i = 1;

    while (i <= n) {
        idx_t q    = n / i;
        idx_t last = n / q;
        idx_t len  = last - i + 1;

        seq_t qseq = (seq_t)q;
        seq_t len_s = (seq_t)len;

        /* sum of floor(n/i) over the block */
        sumB += qseq * len_s;

        /* sum of i over the block */
        seq_t sumi = ((seq_t)last + (seq_t)i) * len_s / 2;
        sumA += qseq * sumi;

        i = last + 1;
    }

    seq_t tri = ((seq_t)(n - 1)) * (seq_t)n / 2;
    return sumA - sumB - tri;
}

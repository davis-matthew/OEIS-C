/*
 * A239186  Sum of the largest two parts in the partitions of 4n into 4 parts
 *          with smallest part equal to 1.
 * https://oeis.org/A239186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 967, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A239186: Sum of the largest two parts in the partitions of 4n into 4 parts with smallest part equal to 1.
    a(n) = Σ_{x1≥x2≥x3≥1, x1+x2+x3=4n‑1} (x1+x2)
    = (4n‑1) * P(4n‑1) – Σ_{t=1}^{⌊(4n‑1)/3⌋} t * N(t)
    where N(t) = ⌊((4n‑1)‑t)/2⌋ – t + 1  (number of partitions with smallest part t)
    and P(4n‑1) = Σ_{t} N(t)   (total number of partitions) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */
    idx_t m = (idx_t)4 * n - (idx_t)1;   /* m = 4n - 1 */
    idx_t tmax = m / 3;

    seq_t total_P = 0;
    seq_t Sx3 = 0;

    for (idx_t t = 1; t <= tmax; ++t) {
        idx_t tmp = (m - t) / 2;                /* floor((m - t)/2)  */
        seq_t count = (seq_t)tmp - (seq_t)t + (seq_t)1;
        total_P += count;
        Sx3 += (seq_t)t * count;
    }

    seq_t res = total_P * (seq_t)m - Sx3;
    return res;
}

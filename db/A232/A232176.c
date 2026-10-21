/*
 * A232176  Least positive k such that n^2 + triangular(k) is a square.
 * https://oeis.org/A232176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that n² + k(k+1)/2 is a square
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nsq = (seq_t)n * (seq_t)n;        /* n² */
    idx_t max_k = 4 * n - 2;                /* a(n) ≤ 4n − 2 */

    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t tri = (seq_t)k * (seq_t)(k + 1) / 2;   /* k(k+1)/2 */
        seq_t s   = nsq + tri;                       /* target */
        seq_t m   = floor_sqrt(s);                   /* ⌊√s⌋ */
        if (m * m == s)                              /* exact square */
            return (seq_t)k;
    }
    /* Theoretically unreachable: lower bounds guarantee a solution. */
    return (seq_t)0;
}

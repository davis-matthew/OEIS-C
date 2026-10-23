/*
 * A100452  Triangle read by rows, based on array described below.
 * https://oeis.org/A100452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   a(n) derives from the triangle a(m,n) with a(1,n)=n² and
 *            a(m+1,n)=(n‑m)·⌊(a(m,n)-1)/(n‑m)⌋; the sequence is read by
 *            columns with rows reversed (m=n,…,1)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Locate the column k (the value of n in the triangle) */
    seq_t k = 1;
    while ((k * (k + 1)) / 2 < (seq_t)n) ++k;

    /* Number of terms before column k */
    seq_t prev = (k * (k - 1)) / 2;

    /* Position inside column k (1‑based) and corresponding row m */
    seq_t s = (seq_t)n - prev;   /* 1 ≤ s ≤ k */
    seq_t m = k - s + 1;         /* 1 ≤ m ≤ k */

    /* Compute a(m,k) by the recurrence, starting from a(1,k)=k² */
    seq_t val = k * k;           /* a(1,k) */
    for (seq_t i = 1; i < m; ++i) {
        seq_t div = k - i;       /* (n - m) in the recurrence */
        val = div * ((val - 1) / div);
    }
    return val;
}

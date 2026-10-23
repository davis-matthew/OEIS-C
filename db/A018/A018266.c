/*
 * A018266  Divisors of 60.
 * https://oeis.org/A018266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018266: divisors of 60 in increasing order
   Formula: a(n) = n + floor((n-1)/6)*(60/(13-n)-n)
   (valid for 1 ≤ n ≤ 12) */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* compute all divisors of 60 */
    const seq_t NUM = 60;
    seq_t d_small[8]  = {0};   /* divisors ≤ sqrt(NUM) */
    seq_t d_large[8]  = {0};   /* conjugate divisors > sqrt(NUM) */
    size_t small_cnt = 0, large_cnt = 0;

    for (idx_t i = 1; i * i <= NUM; ++i) {
        if (NUM % i == 0) {
            d_small[small_cnt++] = (seq_t)i;
            if (i != NUM / i) {
                d_large[large_cnt++] = NUM / i;
            }
        }
    }

    size_t total = small_cnt + large_cnt;

    /* out of range, return 0 */
    if (n == 0 || n > total) {
        return (seq_t)0;
    }

    if ((idx_t)n <= small_cnt) {
        return d_small[n - 1];
    } else {
        /* index into the large part counted from the largest divisor */
        idx_t idx = n - small_cnt;          /* 1‑based */
        return d_large[large_cnt - idx];   /* reverse to get ascending order */
    }
}

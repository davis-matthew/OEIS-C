/*
 * A036059  The summarize Fibonacci sequence: summarize the previous two
 *          terms!.
 * https://oeis.org/A036059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 181, u128 n <= 181,
 *            big any n
 * Helpers:   counts_of_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1, a(1) = 1
 * Recurrence: For n >= 2,
 *   a(n) = concat_{d=9..0, c_d>0} (c_d * 10 + d), 
 *   where c_d is the count of digit d (0‑9) in the decimal representations of
 *   a(n−1) and a(n−2).  The concatenation is performed with the digits
 *   sorted in descending order.
 */
#include <stddef.h>
#include <stdint.h>
#include "../utils/counts_of_seq.h"

/* helper: number of decimal digits of a non‑zero value */
static int digit_len_seq_t(seq_t v)
{
    int d = 0;
    do {
        ++d;
        v /= (seq_t)10;
    } while (v);
    return d;
}

seq_t a(idx_t n)
{
    /* handle the two base cases */
    if (n == 0 || n == 1) return (seq_t)1;

    seq_t val0 = (seq_t)1;       /* a(0) */
    seq_t val1 = (seq_t)1;       /* a(1) */
    uint64_t cnt0[10], cnt1[10];
    counts_of_seq(val0, cnt0);
    counts_of_seq(val1, cnt1);

    for (idx_t i = 2; i <= n; ++i) {
        uint64_t sum_cnt[10];
        for (int d = 0; d < 10; ++d) sum_cnt[d] = cnt0[d] + cnt1[d];

        /* build a(i) from the summed counts */
        seq_t new_val = (seq_t)0;
        for (int d = 9; d >= 0; --d) {
            uint64_t c = sum_cnt[d];
            if (c == 0) continue;

            int seg_len = digit_len_seq_t((seq_t)c) + 1;        /* count digits + digit */
            seq_t pow10 = (seq_t)1;
            for (int k = 0; k < seg_len; ++k) pow10 *= (seq_t)10;

            seq_t seg = (seq_t)c * (seq_t)10 + (seq_t)d;
            new_val = new_val * pow10 + seg;
        }

        /* shift values for next iteration */
        val0 = val1;      /* a(n‑1) becomes a(n‑2) */
        val1 = new_val;   /* a(n) becomes a(n‑1) */

        /* update digit counts */
        for (int d = 0; d < 10; ++d) cnt0[d] = cnt1[d];
        counts_of_seq(val1, cnt1);
    }

    return val1;
}

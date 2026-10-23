/*
 * A192118  Monotonic ordering of nonnegative differences 2^i-7^j, for
 *          40>=i>=0, j>=0.
 * https://oeis.org/A192118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest distinct value among * all nonnegative
 *            differences 2^i - 7^j with 0 <= i <= 40 and j >= 0
 * Range:     u8 n <= 8, u16 n <= 38, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   pow_seq, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"
#include "../utils/seq_cmp.h"

seq_t a(idx_t n)
{
    /* The sequence is finite: i is bounded by 40.
     * We precompute all differences once and sort them. */
    static seq_t values[400];
    static size_t count = 0;
    static bool ready = false;
    if (!ready) {
        /* Generate all nonnegative differences. */
        size_t tmp = 0;
        for (idx_t i = 0; i <= 40; ++i) {
            seq_t p2 = pow_seq(2, (int)i);           /* 2^i  */
            for (idx_t j = 0; ; ++j) {
                seq_t p7 = pow_seq(7, (int)j);       /* 7^j  */
                if (p7 > p2) break;                 /* p2 - p7 would become negative */
                values[tmp++] = p2 - p7;             /* keep nonnegative difference */
            }
        }

        /* Sort the values. */
        qsort(values, tmp, sizeof(seq_t), seq_cmp);

        /* Remove duplicates. */
        if (tmp > 0) {
            size_t uniq = 1;
            seq_t prev = values[0];
            for (size_t i = 1; i < tmp; ++i) {
                if (values[i] != prev) {
                    prev = values[i];
                    values[uniq++] = prev;
                }
            }
            count = uniq;
        } else {
            count = 0;
        }
        ready = true;
    }

    if (n == 0 || n > count) {
        /* Undefined beyond the sequence length; return 0. */
        return 0;
    }
    return values[n - 1];
}

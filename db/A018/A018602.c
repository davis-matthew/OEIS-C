/*
 * A018602  Divisors of 708.
 * https://oeis.org/A018602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n‑th divisor of 708 in increasing order (0 is
 *            returned for n outside 1..12)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    return (*x > *y) - (*x < *y);
}

seq_t a(idx_t n)
{
    static seq_t divisors[12];
    static bool ready = false;

    if (!ready) {
        int pos = 0;
        for (uint64_t e2 = 0; e2 <= 2; ++e2) {
            for (uint64_t e3 = 0; e3 <= 1; ++e3) {
                for (uint64_t e59 = 0; e59 <= 1; ++e59) {
                    seq_t val = (seq_t)1;
                    for (uint64_t i = 0; i < e2; ++i) val *= (seq_t)2;
                    for (uint64_t i = 0; i < e3; ++i) val *= (seq_t)3;
                    for (uint64_t i = 0; i < e59; ++i) val *= (seq_t)59;
                    divisors[pos++] = val;
                }
            }
        }
        qsort(divisors, pos, sizeof(seq_t), cmp_seq);
        ready = true;
    }

    if (n == 0 || n > 12) return (seq_t)0;
    return divisors[(size_t)(n - 1)];
}

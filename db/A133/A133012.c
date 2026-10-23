/*
 * A133012  Even imperfect numbers.
 * https://oeis.org/A133012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Even imperfect numbers: all even positive integers that are not perfect numbers. */
/* For n >= 1, a(n) is the nth even integer that is not a perfect number. */
seq_t a(idx_t n)
{
    static const seq_t perfects[] = {
        (seq_t)6ULL,
        (seq_t)28ULL,
        (seq_t)496ULL,
        (seq_t)8128ULL,
        (seq_t)33550336ULL,
        (seq_t)8589869056ULL,
        (seq_t)137438691328ULL,
        (seq_t)2305843008139952128ULL
    };
    const idx_t perfects_cnt = sizeof(perfects) / sizeof(perfects[0]);

    idx_t count = 0;
    seq_t val = (seq_t)2;

    while (1) {
        bool is_perfect = false;
        for (idx_t i = 0; i < perfects_cnt; ++i) {
            if (perfects[i] == val) {
                is_perfect = true;
                break;
            }
            if (perfects[i] > val) {
                break;
            }
        }
        if (!is_perfect) {
            ++count;
            if (count == n)
                return val;
        }
        val += (seq_t)2;
    }
}

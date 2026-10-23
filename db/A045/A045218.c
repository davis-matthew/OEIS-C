/*
 * A045218  Numbers whose base-5 representation contains no 1's and exactly
 *          one 2.
 * https://oeis.org/A045218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   power3, suffix_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A045218: numbers whose base‑5 representation contains no 1's
   and exactly one 2. */
#include <stdbool.h>
#include "../utils/power3.h"
#include "../utils/suffix_count.h"

seq_t a(idx_t n)
{
    /* Find the length L of the n‑th term. */
    idx_t L = 1;
    seq_t cumulative = 1;          /* count(1) */
    seq_t countL = 1;              /* count for current L */

    while (cumulative < n) {
        L++;
        if (L == 1) {
            countL = 1;
        } else {
            countL = (seq_t)(2 * L + 1) * power3(L - 2);
        }
        cumulative += countL;
    }

    seq_t prev_cumulative = cumulative - countL;
    seq_t r = n - prev_cumulative - 1;   /* 0‑based rank within length L */

    /* Pre‑defined digit sets. */
    static const seq_t digits_1[3]  = {2, 3, 4};
    static const seq_t digits_no2[3] = {0, 3, 4};
    static const seq_t digits_all[4] = {0, 2, 3, 4};

    seq_t result = 0;
    bool placed2 = false;

    for (idx_t pos = 1; pos <= L; ++pos) {
        idx_t rem = L - pos;
        const seq_t *digits;
        int num_digits;

        if (pos == 1) {
            digits = digits_1;
            num_digits = 3;
        } else if (placed2) {
            digits = digits_no2;
            num_digits = 3;
        } else {
            digits = digits_all;
            num_digits = 4;
        }

        for (int di = 0; di < num_digits; ++di) {
            seq_t d = digits[di];
            bool placed2_next = placed2 || (d == 2);
            seq_t cnt = suffix_count(rem, placed2_next);
            if (cnt <= r) {
                r -= cnt;
                continue;
            }
            result = result * 5 + d;
            placed2 = placed2_next;
            break;
        }
    }
    return result;
}

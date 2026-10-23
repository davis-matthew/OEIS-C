/*
 * A045067  Numbers whose base-4 representation contains exactly one 0 and one
 *          3.
 * https://oeis.org/A045067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 4861, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045067: Numbers whose base‑4 representation contains exactly one 0 and one 3 */

#include <stdint.h>
#include <stdbool.h>

static void count_completions(idx_t rem, bool need0, bool need3, idx_t *cnt)
{
    /* Count the number of ways to fill the remaining positions `rem`
       when `need0` indicates that a zero must still be placed
       and `need3` indicates that a three must still be placed. */
    if (rem == 0) {
        *cnt = (need0 || need3) ? 0 : 1;
        return;
    }
    if (need0 && need3) {
        if (rem < 2) { *cnt = 0; return; }
        *cnt = rem * (rem - 1) * ((idx_t)1 << (rem - 2));
        return;
    }
    if (need0 || need3) {
        *cnt = rem * ((idx_t)1 << (rem - 1));
        return;
    }
    *cnt = ((idx_t)1 << rem);
}

seq_t a(idx_t n)
{
    /* Find the length L of the base‑4 representation of a(n). */
    idx_t cum = 0;
    idx_t L = 2;
    idx_t countL;
    while (1) {
        countL = (L - 1) * (L - 1) * ((idx_t)1 << (L - 2));
        if (cum + countL >= n) break;
        cum += countL;
        ++L;
    }
    /* Zero‑based offset inside numbers of length L. */
    idx_t offset = n - cum - 1;

    seq_t value = 0;
    bool used0 = false, used3 = false;

    for (idx_t pos = 1; pos <= L; ++pos) {
        /* Iterate digits in ascending order. */ 
        for (uint8_t d = (pos == 1) ? 1 : 0; d <= 3; ++d) {
            if (d == 0 && used0) continue;
            if (d == 3 && used3) continue;

            bool next_used0 = used0 || (d == 0);
            bool next_used3 = used3 || (d == 3);
            idx_t rem = L - pos;
            bool need0 = !next_used0;
            bool need3 = !next_used3;
            idx_t cnt;
            count_completions(rem, need0, need3, &cnt);

            if (offset < cnt) {
                value = value * 4 + d;
                used0 = next_used0;
                used3 = next_used3;
                break;               /* digit chosen, move to next position */
            } else {
                offset -= cnt;        /* skip all numbers starting with this digit */
            }
        }
    }
    return value;
}

/*
 * A037192  Summarize the previous term!, starting with 8899.
 * https://oeis.org/A037192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   count_each_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 8899.
 * For n >= 0,
 *  let S = a(n).
 *  Count the occurrences of each decimal digit d in S.
 *  Form a(n+1) by concatenating, for d = 0..9, in ascending order:
 *      decimal representation of count(d), followed by the digit d
 *      (only for digits where count(d) > 0).
 * This implements the “look‑and‑count” rule described in the OEIS entry.
 *
 * The implementation builds every term from the previous one using only
 * integer arithmetic; the term length never exceeds 20 digits, so all
 * calculations fit comfortably in seq_t and complete in O(n) time.
 */

#include <string.h>   /* for memset */
#include "../utils/count_each_digit.h"

seq_t a(idx_t n)
{
    /* Handle the initial term directly. */
    if (n == 0) return 8899;

    /* Current term digits.  Length never exceeds a small constant. */
    int curr[32], next[32];
    int clen = 4;
    curr[0] = 8; curr[1] = 8; curr[2] = 9; curr[3] = 9;

    int cnt[10];
    for (idx_t step = 0; step < n; ++step) {
        count_each_digit(curr, clen, cnt);

        int nlen = 0;
        for (int d = 0; d <= 9; ++d) {
            if (cnt[d] == 0) continue;

            /* Convert count to decimal digits. */
            int tmp[10];
            int tlen = 0;
            int c = cnt[d];
            do {
                tmp[tlen++] = c % 10;
                c /= 10;
            } while (c);

            /* Append the digits of the count (MSB first). */
            for (int j = tlen - 1; j >= 0; --j)
                next[nlen++] = tmp[j];

            /* Append the digit itself. */
            next[nlen++] = d;
        }

        /* Prepare for the next iteration. */
        memcpy(curr, next, nlen * sizeof(int));
        clen = nlen;
    }

    /* Convert the final digits to a single integer value. */
    seq_t result = 0;
    for (int i = 0; i < clen; ++i) {
        result = result * 10 + curr[i];
    }
    return result;
}

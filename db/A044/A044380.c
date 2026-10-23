/*
 * A044380  Numbers n such that string 4,8 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044380 (OEIS):
 *  Numbers n such that the decimal string of n contains the substring "48"
 *  but the decimal string of n-1 does not.
 *  This implementation scans successive integers, tests the condition,
 *  and counts qualifying numbers until the desired index is reached.
 */

#include "../utils/seq_to_str.h"
#include <string.h>

seq_t a(idx_t n)
{
    /* If n is 0 or negative, behaviour is undefined; the sequence
       is defined for n >= 1. */
    idx_t count = 0;
    for (idx_t i = 48; ; ++i) {
        seq_t cur  = (seq_t)i;
        seq_t prev = (seq_t)(i - 1);

        /* Convert current and previous numbers to decimal strings. */
        char buf_cur[128];
        char buf_prev[128];
        size_t len_cur, len_prev;

        seq_to_str(cur,  buf_cur,  &len_cur);
        seq_to_str(prev, buf_prev, &len_prev);

        /* Null‑terminate the buffers so that strstr can be used. */
        buf_cur[len_cur]  = '\0';
        buf_prev[len_prev] = '\0';

        /* Check if cur contains "48" and prev does not. */
        if (strstr(buf_cur, "48") && !strstr(buf_prev, "48")) {
            count++;
            if (count == n) {
                return cur;
            }
        }
    }
}

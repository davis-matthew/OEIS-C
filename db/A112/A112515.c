/*
 * A112515  Say what you see in previous term, same as A063850 but starting
 *          with 5.
 * https://oeis.org/A112515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A112515: "say what you see in the previous term", starting with 5.
   Recurrence: a(1) = 5. For n > 1, let cur = a(n‑1).  For each distinct
   digit d that appears in cur, in the order of its first appearance,
   let c(d) be the total number of occurrences of d in cur.  Then
   a(n) is the concatenation of the decimal expansion of c(d) followed
   by d for each such digit d.
   This function implements that recurrence.  It works for any
   unsigned integer type seq_t (uint8_t … uint64_t, __int128, or an
   arbitrary‑precision integer class) and for any idx_t (usually
   uint64_t).  No external dependencies other than the standard C
   headers and ../utils/seq_to_dec_string.h are required.
*/
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Convert a seq_t value to a decimal string.  The buffer must be
   large enough to hold the result.  */
static void seq_to_dec_string(seq_t val, char *buf, size_t buflen)
{
    seq_t v = val;
    if (v == 0) {
        if (buflen >= 2) {
            buf[0] = '0';
            buf[1] = '\0';
        }
        return;
    }
    size_t pos = 0;
    /* Extract digits in reverse order. */
    while (v != 0 && pos + 1 < buflen) {
        seq_t d = v % 10;
        v = v / 10;
        buf[pos++] = (char)('0' + (int)d);
    }
    /* Null‑terminate and reverse the string. */
    if (pos >= buflen) pos = buflen - 1;
    buf[pos] = '\0';
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = tmp;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t cur = 5;
    if (n == 1)
        return cur;

    /* Buffers are large enough for all terms up to n = 10000 */
    char cur_buf[8192];
    char next_buf[8192];

    for (idx_t step = 2; step <= n; ++step) {
        /* Convert current term to a decimal string */
        seq_to_dec_string(cur, cur_buf, sizeof(cur_buf));

        /* Count occurrences of each digit and remember first appearance */
        int cnt[10] = {0};
        int first[10];
        for (int d = 0; d < 10; ++d)
            first[d] = -1;

        int order_digits[10];
        int order_len = 0;

        for (int i = 0; cur_buf[i] != '\0'; ++i) {
            int d = cur_buf[i] - '0';
            cnt[d]++;

            if (first[d] == -1) {
                first[d] = order_len;
                order_digits[order_len++] = d;
            }
        }

        /* Build next term by iterating digits in first‑appearance order */
        int pos = 0;
        for (int k = 0; k < order_len; ++k) {
            int d = order_digits[k];
            int c = cnt[d];

            /* Convert count c to decimal digits, stored in reverse */
            int tmp[32];
            int tmpp = 0;
            if (c == 0) {
                tmp[tmpp++] = '0';
            } else {
                while (c > 0) {
                    tmp[tmpp++] = '0' + (c % 10);
                    c /= 10;
                }
            }
            for (int m = tmpp - 1; m >= 0; --m)
                next_buf[pos++] = (char)tmp[m];
            next_buf[pos++] = '0' + d;
        }
        next_buf[pos] = '\0';

        /* Convert the next string back to seq_t */
        seq_t next = 0;
        for (int j = 0; j < pos; ++j) {
            next = next * 10 + (seq_t)(next_buf[j] - '0');
        }

        cur = next;
    }

    return cur;
}

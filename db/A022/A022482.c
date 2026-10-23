/*
 * A022482  Describe previous term from the right (method A - initial term is
 *          2).
 * https://oeis.org/A022482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   seq_from_str, to_decimal_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2, a(n+1) = A045918(A004086(a(n))) */
/* Implements the "describe previous term from the right" (Method A). */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/to_decimal_string.h"

seq_t a(idx_t n)
{
    /* Special case: first term */
    if (n == 1) {
        return seq_from_str("2");
    }

    /* Allocate the initial string "2" */
    char *prev = (char *)malloc(2);
    if (!prev) return 0;   /* fallback in case of OOM */
    prev[0] = '2';
    prev[1] = '\0';
    size_t prev_len = 1;
    char *curr = NULL;

    for (idx_t i = 2; i <= n; ++i) {
        /* Allocate enough space: worst case 3 chars per input char + some guard. */
        size_t alloc_size = prev_len * 3 + 32;
        curr = (char *)malloc(alloc_size);
        if (!curr) {
            free(prev);
            return 0;   /* fallback */
        }

        size_t out_pos = 0;
        size_t pos = prev_len;
        while (pos > 0) {
            char digit = prev[pos - 1];
            size_t count = 1;
            pos--;
            while (pos > 0 && prev[pos - 1] == digit) {
                count++;
                pos--;
            }
            /* Convert count to decimal string */
            char count_buf[32];
            size_t len_count = to_decimal_string(count, count_buf);
            memcpy(curr + out_pos, count_buf, len_count);
            out_pos += len_count;
            curr[out_pos++] = digit;
        }
        curr[out_pos] = '\0';

        free(prev);
        prev = curr;
        prev_len = out_pos;
    }

    seq_t result = seq_from_str(prev);
    free(prev);
    return result;
}

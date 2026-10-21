/*
 * A052039  a(n) is the smallest k such that the first significant digits of
 *          1/k coincide with n.
 * https://oeis.org/A052039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A052039: a(n) is the smallest k such that the first significant digits
   of 1/k coincide with n (leading zeros are ignored, trailing zeros from
   terminating decimals are omitted).
   This code implements a brute‑force search with exact integer arithmetic.
*/
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not defined for n < 1 */

    /* Convert n to its decimal digits (most significant first) */
    int temp[20];
    int d = 0;
    idx_t x = n;
    while (x > 0) {
        temp[d++] = x % 10;
        x /= 10;
    }
    int digits[20];
    for (int i = 0; i < d; ++i) digits[i] = temp[d - 1 - i];

    /* Iterate candidates for k */
    for (seq_t k = 1;; ++k) {
        seq_t int_part = 1 / k;
        seq_t rem      = 1 % k;
        int  pos       = 0;
        bool first_nonzero_found = false;

        /* Handle integer part (only possible when k==1) */
        if (int_part != 0) {
            if (int_part != (seq_t)digits[0]) continue;  /* mismatch */
            first_nonzero_found = true;
            pos = 1;
            if (d == 1) return k;        /* exact match with integer part */
            if (rem == 0) continue;      /* decimal part missing */
        }

        /* Process decimal digits */
        while (true) {
            rem *= 10;
            seq_t digit = rem / k;
            rem %= k;

            if (!first_nonzero_found) {
                if (digit != 0) {
                    if (digit != (seq_t)digits[pos]) break;
                    first_nonzero_found = true;
                    pos++;
                    if (pos == d) return k;            /* all matched */
                    if (rem == 0) break;               /* no more digits */
                }
                /* otherwise leading zero – skip */
            } else {
                if (digit != (seq_t)digits[pos]) break;
                pos++;
                if (pos == d) return k;                /* all matched */
                if (rem == 0) break;                   /* no more digits */
            }
        }
        /* not found for this k – try next */
    }
}

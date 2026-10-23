/*
 * A043019  Base-7 palindromes that start with 5.
 * https://oeis.org/A043019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration: base-7 palindromes starting with digit 5, in increasing order */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t count = 0;
    static seq_t left[64];
    static seq_t pow7[64];

    for (idx_t len = 1; ; ++len) {
        idx_t h = (len + 1) / 2;        /* left half length */
        /* compute powers of 7 up to h-1 */
        pow7[0] = (seq_t)1;
        for (idx_t i = 1; i < h; ++i) {
            pow7[i] = pow7[i-1] * (seq_t)7;
        }
        seq_t total = pow7[h-1];        /* number of palindromes of this length */
        for (idx_t idx = 0; idx < total; ++idx) {
            ++count;
            if (count == n) {
                /* build left half digits */
                left[0] = (seq_t)5;
                for (idx_t j = 1; j < h; ++j) {
                    idx_t dpos = h - 1 - j;              /* most significant to least */
                    left[j] = (seq_t)((idx / pow7[dpos]) % 7);
                }
                /* construct full palindrome and convert to decimal */
                seq_t val = 0;
                for (idx_t i = 0; i < len; ++i) {
                    seq_t d = (i < h) ? left[i] : left[len - 1 - i];
                    val = val * (seq_t)7 + d;
                }
                return val;
            }
        }
    }
}

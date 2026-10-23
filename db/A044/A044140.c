/*
 * A044140  Numbers k such that substring "02" occurs in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A044140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: iterate natural numbers in base‑7; keep those where the
   string "02" appears as a contiguous substring but does not appear
   in k‑1.  The nth such number is returned.  The sequence is
   cached so that calls with arbitrary n are handled in O(n) time. */

#include <stdbool.h>
#include <stdlib.h>

/* Check whether base‑7 representation of x contains the substring "02". */
static bool has02(seq_t x)
{
    if (x == 0) return false;
    seq_t prev = x % 7;
    x /= 7;
    while (x > 0) {
        seq_t cur = x % 7;
        if (cur == 0 && prev == 2) return true;
        prev = cur;
        x /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t cap = 0;
    static idx_t built = 0;
    static seq_t cur_k = 0;

    if (n == 0) return 0; /* undefined for n<1, return 0 for safety */

    if (built < n) {
        if (buf == NULL) {
            cap = 64;
            buf = (seq_t *)malloc(cap * sizeof(seq_t));
        }
        while (built < n) {
            if (built == cap) {
                cap <<= 1;
                buf = (seq_t *)realloc(buf, cap * sizeof(seq_t));
            }
            ++cur_k;
            if (has02(cur_k) && !has02(cur_k - 1)) {
                buf[built++] = cur_k;
            }
        }
    }
    return buf[n - 1];
}

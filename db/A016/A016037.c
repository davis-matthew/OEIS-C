/*
 * A016037  Map numbers to number of letters in English name; sequence gives
 *          number of steps to converge (to 4).
 * https://oeis.org/A016037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A016037:
   a(n) = number of iterations required to reach 4 by repeatedly
   replacing the current number by the number of letters in its
   English name (spaces and hyphens ignored).  a(0)=1, a(4)=0. */

#include <stdint.h>

static const int unit_len[10] = {0,3,3,5,4,4,3,5,5,4};          /* units 0‑9 (0 unused) */
static const int len_twenty[20] = {0,3,3,5,4,4,3,5,5,4,3,6,6,8,8,7,7,9,8,8}; /* 0‑19 */
static const int tens_len[10]  = {0,0,6,6,5,5,5,7,6,6};           /* tens 0‑9 (2‑9 used) */
static const int scale_len[10] = {0,8,7,7,8,10,10};              /* group 1‑6 (thousand…) */

static int count_letters_uint64(uint64_t n) {
    if (n == 0) return 4; /* "zero" */
    int len = 0;
    int scale_index = 0;
    while (n > 0) {
        uint64_t group = n % 1000;
        if (group != 0) {
            int hundreds = group / 100;
            if (hundreds) {
                len += unit_len[hundreds] + 7; /* "hundred" */
            }
            int remainder = group % 100;
            if (remainder) {
                if (remainder < 20) {
                    len += len_twenty[remainder];
                } else {
                    int tens = remainder / 10;
                    int units = remainder % 10;
                    len += tens_len[tens];
                    if (units) len += unit_len[units];
                }
            }
            if (scale_index > 0) {
                len += scale_len[scale_index];
            }
        }
        n /= 1000;
        scale_index++;
    }
    return len;
}

seq_t a(idx_t n) {
    seq_t steps = 0;
    uint64_t cur = (uint64_t)n;
    while (cur != 4) {
        cur = (uint64_t)count_letters_uint64(cur);
        steps++;
    }
    return steps;
}

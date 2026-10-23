/*
 * A163125  Sum of digits of the n-th Self-number (or Colombian number),
 *          A003052(n).
 * https://oeis.org/A163125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum( A003052(n) )
 * Range:     u8 n <= 29, u16 n <= 6483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the digits of the n-th Colombian (self) number. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* allocate an array that marks numbers generated so far */
    size_t cap = 1024;
    uint8_t *seen = (uint8_t*)calloc(cap, 1);
    if (!seen)
        return 0;

    idx_t self_cnt = 0;
    idx_t x = 1;

    for (;; ++x)
    {
        /* compute y = x + digit_sum(x) */
        seq_t xs = (seq_t)x;
        seq_t sum = digit_sum(xs);
        seq_t y_seq = xs + sum;
        idx_t y_idx = (idx_t)y_seq;

        /* expand array if needed for y */
        if (y_idx >= cap) {
            while (y_idx >= cap) {
                size_t new_cap = cap << 1;
                uint8_t *tmp = (uint8_t*)realloc(seen, new_cap);
                if (!tmp) { free(seen); return 0; }
                memset(tmp + cap, 0, new_cap - cap);
                seen = tmp;
                cap = new_cap;
            }
        }
        seen[y_idx] = 1;                /* mark number as generated */

        /* check if current x is a self (Colombian) number */
        if (!seen[(size_t)x]) {
            ++self_cnt;
            if (self_cnt == n) {
                seq_t result = digit_sum(xs);
                free(seen);
                return result;
            }
        }

        /* expand array if needed for upcoming x */
        if (x >= cap) {
            while (x >= cap) {
                size_t new_cap = cap << 1;
                uint8_t *tmp = (uint8_t*)realloc(seen, new_cap);
                if (!tmp) { free(seen); return 0; }
                memset(tmp + cap, 0, new_cap - cap);
                seen = tmp;
                cap = new_cap;
            }
        }
    }
}

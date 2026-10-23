/*
 * A031956  Numbers with exactly three distinct base-4 digits.
 * https://oeis.org/A031956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑4 representation
 *            contains exactly three distinct digits
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *vals = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    if (n == 0) return (seq_t)0;  /* offset is 1, return 0 for safety */

    /* grow cache until we have at least n elements */
    while (len < n) {
        /* start searching from the next integer after the last stored one,
           or from 18 for the very first term */
        seq_t candidate = (len == 0) ? (seq_t)18 : vals[len - 1] + (seq_t)1;
        bool ok = false;

        /* find the next integer with exactly three distinct base‑4 digits */
        while (!ok) {
            uint8_t mask = 0;
            seq_t y = candidate;

            while (y > (seq_t)0) {
                uint8_t d = (uint8_t)(y % (seq_t)4);
                mask |= (1u << d);
                if (mask == 0x0F) break;          /* all four digits found – cannot happen but exit */
                y /= (seq_t)4;
            }

            /* count set bits in mask (only 4 bits) */
            uint8_t m = mask;
            int bits = 0;
            while (m) {
                m &= m - 1;  /* clear lowest set bit */
                bits++;
            }
            ok = (bits == 3);
            if (!ok) candidate++;
        }

        /* store the found candidate */
        if (len == cap) {
            size_t newcap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(vals, newcap * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failed */
            vals = tmp;
            cap = newcap;
        }
        vals[len++] = candidate;
    }

    return vals[n - 1];
}

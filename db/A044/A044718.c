/*
 * A044718  Numbers n such that string 0,5 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n with substring 05 in decimal representation of n but
 *            not in n+1
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline bool contains05(seq_t x) {
    if (x < 10) return false;          /* need at least two digits */
    int prev = (int)(x % 10);
    x /= 10;
    while (x > 0) {
        int curr = (int)(x % 10);
        if (curr == 0 && prev == 5) return true;
        prev = curr;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined for offset 1, guard anyway */
    static seq_t *buf = NULL;           /* cache of found terms */
    static size_t cap = 0;
    static idx_t count = 0;             /* number of terms stored */
    static seq_t search = 0;            /* next number to examine */

    if (!buf) {                         /* initialise cache */
        cap = 128;
        buf = mem_new(seq_t, cap);
    }

    while (count < n) {
        /* find next candidate */
        while (true) {
            seq_t cand = search;
            if (contains05(cand) && !contains05(cand + 1)) {
                /* store it */
                if (count == cap) {          /* grow cache */
                    cap *= 2;
                    buf = mem_resize(seq_t, buf, cap);
                }
                buf[count++] = cand;
                search = cand + 1;           /* start after this candidate */
                break;
            }
            search = cand + 1;               /* continue scanning */
        }
    }
    return buf[n - 1];
}

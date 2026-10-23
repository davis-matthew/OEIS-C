/*
 * A044119  Numbers k such that string 2,5 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044119: Numbers k such that the base‑6 representation contains
   the substring "25" but the representation of k‑1 does not. */
#include <stdbool.h>

/* Returns true if x written in base 6 contains the adjacent digits 2 and 5. */
static inline bool has25(seq_t x)
{
    if (x == 0)
        return false;
    /* least significant digit first */
    seq_t last = x % 6;
    x /= 6;
    while (x != 0) {
        seq_t cur = x % 6;
        if (cur == 2 && last == 5)
            return true;
        last = cur;
        x /= 6;
    }
    return false;
}

/* Returns the n-th term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined, return 0 for safety */

    seq_t count = 0;
    seq_t k = 1;                    /* start from 1 */
    bool prev_has = has25(0);       /* base‑6 of 0 does not contain "25" */

    for (;;) {
        bool cur_has = has25(k);
        if (cur_has && !prev_has) {
            ++count;
            if (count == n)
                return k;
        }
        prev_has = cur_has;
        ++k;
    }
}

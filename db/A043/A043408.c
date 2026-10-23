/*
 * A043408  Numbers whose base-7 representation contains exactly four 3's.
 * https://oeis.org/A043408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-7 representation contains exactly four 3's
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: recursively generate numbers of length L in ascending order.
   Returns true when the nth number has been found. */
static bool gen(idx_t L, idx_t pos, idx_t count3, seq_t value,
                idx_t n, idx_t *counter, seq_t *result, bool *found)
{
    if (*found) return true;               /* already found, stop all recursion */

    if (pos == L) {                        /* all digits placed */
        if (count3 == 4) {                 /* exactly four 3's? */
            (*counter)++;
            if (*counter == n) {            /* we found the nth number */
                *result = value;
                *found = true;
                return true;
            }
        }
        return false;
    }

    /* iterate over digits in increasing order (0..6) */
    for (int d = 0; d < 7; ++d) {
        if (pos == 0 && d == 0) continue;          /* no leading zero */

        idx_t newCount3 = count3 + (d == 3);
        if (newCount3 > 4) continue;               /* too many 3's */

        idx_t rem = L - pos - 1;                   /* remaining positions */
        if (newCount3 + rem < 4) continue;         /* cannot reach four 3's */

        seq_t newVal = value * 7 + (seq_t)d;       /* append digit in base-7 */

        if (gen(L, pos + 1, newCount3, newVal,
                n, counter, result, found))
            return true;
    }
    return false;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    idx_t counter = 0;
    seq_t result = 0;
    bool found = false;

    for (idx_t L = 4; ; ++L) {                     /* start with 4 digits */
        if (gen(L, 0, 0, (seq_t)0, n, &counter, &result, &found))
            return result;                         /* n-th term found */
    }
}

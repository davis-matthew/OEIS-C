/*
 * A044561  Numbers n such that string 6,0 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number n whose base‑7 representation contains the
 *            substring "60", * but the base‑7 representation of n+1 does not
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   contains_60
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains_60.h"

/* OEIS A044561 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1 */

    seq_t cand = 0;
    idx_t found = 0;

    for (;;) {
        cand = cand + (seq_t)1;   /* next candidate */

        if (contains_60(cand) && !contains_60(cand + (seq_t)1)) {
            ++found;
            if (found == n)
                return cand;
        }
    }
}

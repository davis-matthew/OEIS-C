/*
 * A044228  Numbers n such that string 5,1 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains_51
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044228:  Numbers n such that the base‑8 representation of n
   contains the substring "51" but the representation of n‑1 does not.
   Implementation: brute‑force search with check for the 51 pattern. */

#include <stdbool.h>
#include "../utils/contains_51.h"

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)              /* behavior below offset is undefined */
        return 0;

    idx_t count = 0;
    seq_t candidate = 1;         /* start searching from 1 */
    bool prev_has = false;       /* n-1 for candidate=1 has no digits */

    while (true) {
        bool cur_has = contains_51(candidate);
        if (cur_has && !prev_has) {
            ++count;
            if (count == n)
                return candidate;
        }
        prev_has = cur_has;
        ++candidate;
    }
}

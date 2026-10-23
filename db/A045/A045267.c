/*
 * A045267  Numbers whose base-5 representation contains exactly one 2 and one
 *          3.
 * https://oeis.org/A045267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   has_one2_one3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search of numbers whose base‑5 representation contains
 * exactly one digit 2 and exactly one digit 3. */

#include <stdbool.h>
#include "../utils/has_one2_one3.h"

/* OEIS A045267: numbers whose base‑5 representation contains exactly one 2
 * and one 3. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, guard against n==0 */
    idx_t found = 0;
    for (seq_t v = 1; ; ++v) {      /* iterate natural numbers */
        if (has_one2_one3(v)) {
            ++found;
            if (found == n) return v;
        }
    }
}

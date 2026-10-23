/*
 * A044223  Numbers n such that string 4,4 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose base‑8 representation contains *
 *            the substring "44" while its predecessor does not. * * The
 *            implementation simply scans from 1 upward, counting valid *
 *            candidates until the desired index is reached. It works for all
 *            * supported seq_t types (built‑in unsigned integers...
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_44_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_44_base8.h"

/* Return the nth term of sequence A044223 (OEIS). */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for offset 1, guard against underflow */
        return (seq_t)0;

    idx_t count = 0;
    idx_t num   = 0;   /* current integer being tested */

    while (count < n) {
        ++num;                              /* move to next number */

        if (has_44_base8(num)) {             /* contains "44" ? */
            /* ensure predecessor does NOT contain "44" */
            if (!has_44_base8(num - 1)) {
                ++count;                    /* new valid term found */
                if (count == n)
                    return (seq_t)num;       /* cast to seq_t for return */
            }
        }
    }

    /* Control never reaches here for n ≥ 1, but return 0 to silence
     * compiler warnings. */
    return (seq_t)0;
}

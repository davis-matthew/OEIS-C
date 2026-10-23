/*
 * A031986  Numbers with exactly five distinct base-9 digits.
 * https://oeis.org/A031986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   distinct_in_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A031986 – Numbers with exactly five distinct base‑9 digits.
   The sequence is defined by: a(n) is the n‑th positive integer whose
   representation in base 9 contains exactly five distinct digits.    */
#include <stdint.h>   /* for uintptr_t etc.  */
#include "../utils/distinct_in_base9.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* safety for out‑of‑range indices */
        return 0;

    seq_t x = 1;                /* start searching from 1          */
    idx_t seen = 0;             /* how many qualifying numbers found */

    while (seen < n) {
        if (distinct_in_base9(x) == 5) {
            ++seen;
            if (seen == n)
                return x;
        }
        ++x;
    }
    /* The loop always returns, but the compiler might warn about
       reaching end of non-void function. */
    return 0;
}

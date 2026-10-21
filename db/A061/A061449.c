/*
 * A061449  a(1) = 1; a(n+1) = smallest square > a(n) with leading digit equal
 *          to final digit of a(n) and final digit not 0.
 * https://oeis.org/A061449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061449
   a(1) = 1;
   a(n+1) = smallest square > a(n) with leading digit equal to the final digit of a(n)
            and final digit not 0.
   This implementation searches forward from the current value, caching only
   the previous term and its square root.  No memoisation beyond a single
   recursion level is used, so any n >= 1 can be computed in O(n) time. */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n <= 0 */

    seq_t prev_val  = (seq_t)1;   /* a(1) */
    seq_t root      = (seq_t)1;   /* sqrt(a(1)) */

    if (n == 1) return prev_val;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t last_digit = prev_val % (seq_t)10;
        seq_t val = 0;

        /* Find next square with required properties */
        for (;;) {
            ++root;                           /* consider next root */
            val = root * root;                /* candidate square */

            if ((val % (seq_t)10) == (seq_t)0) /* cannot end with 0 */
                continue;

            int d  = digit_len(val);                 /* number of decimal digits */
            seq_t pow10d = pow10((idx_t)d - 1);      /* 10^(d-1) */
            seq_t lead = val / pow10d;                /* leading digit */

            if (lead == last_digit)                  /* condition satisfied */
                break;
        }

        prev_val = val;   /* store for next iteration */
    }

    return prev_val;
}

/*
 * A095777  Number of terms generated for n in increasing bases starting at 2
 *          until the last term with only decimal digits is reached, or 0
 *          where the number of such terms would be infinite (n<10).
 * https://oeis.org/A095777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A095777:  Number of successive bases b ≥ 2 for which the base‑b
 * representation of n consists only of decimal digits 0–9.  If n < 10
 * (every base works) the sequence is defined to be 0.  The value a(n)
 * is therefore the number of bases in the run 2,3,4,… that are valid
 * before the first invalid base is encountered.
 *
 * Algorithm: iterate bases from 2 upwards, testing each base by
 * repeatedly dividing n by the base; if any quotient digit is ≥ 10
 * terminate and return the number of bases examined so far.  For
 * n ≥ 10 the first failure occurs at base n + 1, so the loop terminates
 * after at most n iterations.  Complexity is O(n log n) which is fine
 * for n ≤ 10000.\n */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 10)           /* infinite run ⇒ defined as 0 */
        return 0;

    seq_t count = 0;      /* number of successful consecutive bases */

    for (idx_t base = 2; base <= n + 1; ++base) {
        idx_t tmp   = n;
        bool valid  = true;
        while (tmp) {
            idx_t digit = tmp % base;
            if (digit >= 10) {
                valid = false;
                break;
            }
            tmp /= base;
        }
        if (!valid)
            break;        /* first base with a non‑decimal digit */
        ++count;          /* base is valid */
    }
    return count;
}

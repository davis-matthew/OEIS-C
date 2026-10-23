/*
 * A045105  Numbers whose base-4 representation contains exactly four 1's and
 *          no 2's.
 * https://oeis.org/A045105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045105: numbers whose base-4 representation contains exactly
 * four 1's and no 2's.
 * The function enumerates integers in increasing order and returns
 * the n-th suitable number.  It works for any n ≥ 1 and is efficient
 * enough for n up to ~10 000.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */

    idx_t found = 0;
    seq_t x = 0;

    while (found < n) {
        ++x;                           /* next candidate   */
        seq_t t = x;
        unsigned ones = 0;
        bool ok = true;

        while (t > 0) {
            seq_t r = t % (seq_t)4;    /* digit in base 4   */
            if (r == (seq_t)1) {
                ++ones;
                if (ones > 4) { ok = false; break; }
            } else if (r == (seq_t)2) {
                ok = false;             /* disallowed digit  */
                break;
            }
            t /= (seq_t)4;              /* move to next digit*/
        }

        if (ok && ones == 4) {
            ++found;                    /* valid number found*/
        }
    }
    return x;
}

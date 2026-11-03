/*
 * A043900  Numbers k such that 0 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the base‑6 representation of
 *            k contains the adjacent digits 5 and 0, while the representation
 *            of k+1 does not
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x (written in base 6) has the adjacent pair 5‑0 or 0‑5. */
static bool has_adjacent_5_0(seq_t x)
{
    if (x < (seq_t)6) return false;          /* one digit cannot contain a pair */
    seq_t prev = x % (seq_t)6;               /* least significant digit */
    x = x / (seq_t)6;
    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)6;
        if ((prev == (seq_t)0 && cur == (seq_t)5) ||
            (prev == (seq_t)5 && cur == (seq_t)0))
            return true;
        prev = cur;
        x = x / (seq_t)6;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n = 0 */

    idx_t found = 0;
    seq_t k = (seq_t)0;

    while (found < n) {
        if (has_adjacent_5_0(k)) {
            /* need to check k+1 */
            seq_t kp1 = k + (seq_t)1;
            if (!has_adjacent_5_0(kp1)) {
                ++found;
                if (found == n) return k;
            }
        }
        k = k + (seq_t)1;                     /* go to next integer */
    }
    /* control never reaches here */
    return (seq_t)0;
}

/*
 * A043232  Numbers k such that 1 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation of OEIS A043232:
 * a(n) is the nth integer k such that decimal digits '1' and '9'
 * appear next to each other in k, but not in k-1.
 * The function iteratively tests successive integers until
 * n such numbers are found.  It works for all supported seq_t types. */

#include <stdbool.h>

static bool adjacent_1_9(seq_t x)
{
    /* Need at least two digits to have a pair */
    if (x < 10) return false;
    seq_t last = x % 10;
    seq_t rest = x / 10;
    while (rest > 0) {
        seq_t d = rest % 10;
        if ((last == 1 && d == 9) || (last == 9 && d == 1))
            return true;
        last = d;
        rest = rest / 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    seq_t count = 0;
    seq_t k = 1;
    while (true) {
        if (adjacent_1_9(k) && !adjacent_1_9(k - 1)) {
            count++;
            if (count == n) return k;
        }
        k++;
    }
}

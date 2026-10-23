/*
 * A044755  Numbers n such that string 4,2 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth integer whose decimal representation contains
 * the substring "42" but the integer n+1 does not contain "42".
 * This is sequence A044755. */

#include <stdbool.h>

/* Helper: does integer x (decimal) contain the substring "42"? */
static bool has42(seq_t x)
{
    seq_t prev = 10;          /* sentinel not equal to any digit */
    while (x > 0) {
        seq_t cur = x % 10;   /* current least significant digit */
        if (prev == 2 && cur == 4) {
            return true;      /* found "42" in the original order */
        }
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n is 1-based.  We iterate in a "stateless" manner; the cost
     * is below 2 s for n up to about 10 000. */
    if (n == 0) {            /* undefined below the offset */
        return 0;
    }

    seq_t cur  = 1;          /* candidate number */
    idx_t  cnt = 0;          /* how many valid numbers found */

    while (cnt < n) {
        if (has42(cur)) {
            seq_t next = cur + 1;
            if (!has42(next)) {
                ++cnt;               /* this is the next valid term */
                if (cnt == n) {
                    return cur;      /* return the nth term */
                }
            }
        }
        ++cur;                        /* move to next integer */
    }
    /* unreachable */
    return 0;
}

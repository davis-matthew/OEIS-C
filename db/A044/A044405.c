/*
 * A044405  Numbers n such that string 7,3 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose decimal representation *
 *            contains the consecutive digits "73", while (n-1) does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* true iff x contains the substring "73" */
static bool has73(seq_t x)
{
    if (x < 10) return false;          /* single digit cannot contain 73 */

    seq_t last = x % 10;
    x        /= 10;
    seq_t cur;

    while (x > 0) {
        cur = x % 10;
        if (cur == 7 && last == 3) return true;
        last = cur;
        x    /= 10;
    }
    return false;
}

/* External linkage */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* defined only for n ≥ 1 */

    idx_t found = 0;
    seq_t cand  = 1;          /* start from 1; 0 never qualifies */

    while (1) {
        if (has73(cand) && !has73(cand - 1)) {
            ++found;
            if (found == n)
                return cand;
        }
        ++cand;              /* move to next integer to test */
    }
}

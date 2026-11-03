/*
 * A043219  Numbers k such that 0 and 4 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043219
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

/* Definition: a(n) = the n-th positive integer k such that
   decimal representation of k contains at least one pair of
   adjacent digits 0 and 4 (in any order), and decimal representation
   of k-1 does NOT contain such a pair. */
#include <stdbool.h>

/* Helper: return true iff x contains an adjacent pair of digits
   0 and 4 (either 04 or 40). */
static bool has_adjacent_04(seq_t x)
{
    seq_t prev = (seq_t)10;          /* a value not 0 or 4 */
    seq_t tmp  = x;
    while (tmp > (seq_t)0) {
        seq_t cur = tmp % (seq_t)10;
        if ((prev == (seq_t)0 && cur == (seq_t)4) ||
            (prev == (seq_t)4 && cur == (seq_t)0))
            return true;
        prev = cur;
        tmp  = tmp / (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t  count = (idx_t)0;
    seq_t  candidate = (seq_t)0;
    while (count < n) {
        candidate = candidate + (seq_t)1;
        if (has_adjacent_04(candidate) &&
            !has_adjacent_04(candidate - (seq_t)1)) {
            count++;
        }
    }
    return candidate;
}

/*
 * A044503  Numbers n such that string 3,2 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th integer whose base‑6 representation
   contains the substring “32” while the integer + 1 does not.
   The search starts at 1 and increments by 1 until n such numbers are
   found.  No memoisation is required for the expected range of n. */
#include <stdbool.h>

/* Returns true if x, written in base 6, contains the consecutive digits
   3 followed immediately by 2.  The scan is performed from the least
   significant digit upwards; a match occurs when a lower‑order digit
   2 is followed by a higher‑order digit 3. */
static inline bool has32(seq_t x)
{
    bool lower_is_two = false;
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)6);
        if (lower_is_two && d == 3)      /* digits …3 2… in base 6 */
            return true;
        lower_is_two = (d == 2);
        x = x / (seq_t)6;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* sequence is offset‑1; undefined below 1 */
        return (seq_t)0;

    idx_t  found = 0;
    seq_t  cur   = (seq_t)1;    /* start from 1 */

    while (found < n) {
        if (has32(cur) && !has32(cur + (seq_t)1)) {
            ++found;
            if (found == n)
                return cur;
        }
        ++cur;
    }
    return (seq_t)0;   /* never reached for n >= 1 */
}

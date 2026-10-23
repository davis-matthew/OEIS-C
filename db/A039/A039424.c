/*
 * A039424  Numbers whose base-10 representation has the same nonzero number
 *          of 4's and 6's.
 * https://oeis.org/A039424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose decimal representation
 *            contains equal, non‑zero numbers of the digits 4 and 6
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Recursive generator: builds numbers of a fixed length in ascending
   order.  If the nth valid number is found it is stored in *result
   and returns true. */
static bool generate(idx_t pos, idx_t len,
                     int count4, int count6,
                     seq_t value,
                     idx_t target,
                     idx_t *counter,
                     seq_t *result)
{
    if (pos == len) {
        if (count4 == count6 && count4 > 0) {
            (*counter)++;
            if (*counter == target) {
                *result = value;
                return true;
            }
        }
        return false;
    }

    /* Prune if it is impossible to balance the number of 4 and 6
       with the remaining positions. */
    int rem = (int)(len - pos);
    int diff = count4 - count6;
    if (diff > rem || -diff > rem) return false;

    for (int d = 0; d <= 9; ++d) {
        if (pos == 0 && d == 0) continue;        /* no leading zero */
        int nc4 = count4, nc6 = count6;
        if (d == 4) nc4++;
        else if (d == 6) nc6++;
        seq_t new_val = value * (seq_t)10 + (seq_t)d;
        if (generate(pos + 1, len, nc4, nc6, new_val,
                     target, counter, result))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1 */

    idx_t counter = 0;
    seq_t result = 0;

    for (idx_t len = 2; ; ++len) {
        if (generate(0, len, 0, 0, 0, n, &counter, &result))
            return result;
    }
}

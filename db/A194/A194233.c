/*
 * A194233  Smallest number greater than n with exactly the same digits as n
 *          in decimal representation, a(n)=10*n if no such number exists.
 * https://oeis.org/A194233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A194233: Smallest number greater than n with exactly the same digits as n
   in decimal representation (if none, a(n) = 10*n).  In fact the sequence
   is given by returning the largest permutation of the digits that is
   strictly greater than n; if n already has the digits in descending
   order, return 10*n.  */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract digits of n into freq array (0–9). */
    unsigned freq[10] = {0};
    idx_t tmp = n;
    if (tmp == 0) {
        freq[0] = 1;  /* handle n == 0, though offset is 1 */
    } else {
        while (tmp > 0) {
            freq[tmp % 10]++;
            tmp /= 10;
        }
    }

    /* Build the largest permutation of digits (descending order). */
    seq_t max_val = 0;
    for (int d = 9; d >= 0; --d) {
        for (unsigned k = 0; k < freq[d]; ++k) {
            max_val = max_val * 10 + (seq_t)d;
        }
    }

    /* If this permutation is strictly larger than n, return it;
       otherwise - no larger permutation exists - return 10*n. */
    if (max_val > (seq_t)n)
        return max_val;
    else
        return (seq_t)n * 10;
}

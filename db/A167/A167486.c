/*
 * A167486  Choose the prime sum of pair (n,n+1) otherwise choose n (see
 *          comments for details).
 * https://oeis.org/A167486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167486:
   a(1)=3.  Set start=1.  For each term n:
     let s = 2*start+1 (the sum of the pair (start,start+1)).
     If s is prime, output s and set start += 2;
     otherwise output start and set start += 1.
   The process is repeated n times.  The result fits in seq_t. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   // provides bool is_prime(idx_t x)

seq_t a(idx_t n)
{
    if (n == 0) return 0;               // offset is 1; this case is unused
    idx_t start = 1;
    seq_t res = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* sum of the current pair (start, start+1) */
        idx_t sum = 2 * start + 1;

        if (is_prime(sum)) {
            res = (seq_t)sum;
            start += 2;                 /* next pair starts at sum+1 */
        } else {
            res = (seq_t)start;
            start += 1;                 /* next pair starts at start+1 */
        }
    }
    return res;
}

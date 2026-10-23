/*
 * A169968  Positive integers not in A169884.
 * https://oeis.org/A169968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Ω(A169968) = a(n) is the n-th positive integer whose decimal 
   representation contains at least one odd digit and at least one 
   digit that is not a multiple of 5 (i.e. a digit ∈{1,2,3,4,6,7,8,9}).
   This is equivalent to the numbers that are not zero divisors in
   carryless arithmetic modulo 10.
*/

#include <stdbool.h>

static bool has_required_digits(seq_t v) {
    bool hasOdd = false;
    bool hasNon5 = false;
    seq_t t = v;
    while (t != 0) {
        seq_t d = t % 10;
        if (d & 1)            /* odd digit */
            hasOdd = true;
        if (d != 0 && d != 5) /* not 0 or 5 */
            hasNon5 = true;
        if (hasOdd && hasNon5)
            return true;
        t /= 10;
    }
    return false; /* v = 0 has no digits that satisfy both conditions */
}

seq_t a(idx_t n) {
    seq_t cur = 1;
    idx_t count = 0;
    while (true) {
        if (has_required_digits(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}

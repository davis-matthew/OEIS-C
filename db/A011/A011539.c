/*
 * A011539  "9ish numbers": decimal representation contains at least one nine.
 * https://oeis.org/A011539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose decimal representation contains at
 *            least one digit 9
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The 9ish numbers are obtained by scanning the natural numbers
       for the presence of digit 9.  Because the density of such
       numbers tends to 1, the expected value of a(n) is close to n,
       so a simple linear scan runs in O(n) time which easily meets
       the required performance for n up to 10 000. */

    if (n == 0) return 0;          /* offset is 1; 0 is never requested */

    idx_t count = 0;                 /* how many 9ish numbers have been seen */
    seq_t x = 9;                     /* smallest 9ish number */

    for (;; ++x) {
        /* test whether x contains a digit 9 */
        seq_t t = x;
        bool has9 = false;
        while (t > 0) {
            if (t % 10 == 9) { has9 = true; break; }
            t /= 10;
        }

        if (has9) {
            ++count;
            if (count == n) return x;
        }
    }
}

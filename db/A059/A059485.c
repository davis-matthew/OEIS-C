/*
 * A059485  Highest prime factor is greater than 3.
 * https://oeis.org/A059485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer whose largest prime factor is >
 *            3. It is equivalent to: a(n) is the nth integer that is NOT of
 *            the form 2^a * 3^b
 * Range:     u8 n <= 228, u16 n <= 9933, u32 n <= 9933, u64 n <= 9933,
 *            u128 n <= 9933, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Returns the nth term of OEIS A059485.  n is 1‑based. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many qualifying numbers we have found */
    for (seq_t value = 1; ; ++value) {
        seq_t tmp = value;
        /* strip out all factors 2 */
        while (tmp % (seq_t)2 == 0) {
            tmp /= (seq_t)2;
        }
        /* strip out all factors 3 */
        while (tmp % (seq_t)3 == 0) {
            tmp /= (seq_t)3;
        }
        /* if anything remains greater than 1, value has a prime factor > 3 */
        if (tmp > 1) {
            ++count;
            if (count == n) {
                return value;
            }
        }
    }
}

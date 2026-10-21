/*
 * A210940  The prime numbers and their nonprime nearest-neighbors.
 * https://oeis.org/A210940
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

/* Sequence: all positive integers that are either prime or at distance 1
 * from a prime.  The terms are generated in increasing order.  */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t i = 1;
    for (;;) {
        bool flag = false;
        if (is_prime(i))
            flag = true;
        else if (i > 1 && is_prime(i - 1))
            flag = true;
        else if (is_prime(i + 1))
            flag = true;

        if (flag) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
        ++i;
    }
}

/*
 * A166081  Natural numbers that are not the sum of two distinct primes.
 * https://oeis.org/A166081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that cannot be expressed as the
 *            sum of two distinct primes. For n≥1, the first few terms are
 *            1,2,3,4,6,11,17,23,... The algorithm: - Odd numbers m are in the
 *            sequence iff m-2 is not prime. - The only even numbers that are
 *            in the sequence are 1,2,3,4,6. - All other...
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        bool in_seq = false;

        /* Special evens that are in the sequence */
        if (m == 1 || m == 2 || m == 3 || m == 4 || m == 6) {
            in_seq = true;
        }
        /* Odd numbers */
        else if ((m & 1) == 1) {
            idx_t diff = m - 2;          /* no underflow because m>=5 here */
            if (!is_prime(diff)) {
                in_seq = true;
            }
        }
        /* Even numbers >6 are excluded automatically */

        if (in_seq) {
            ++count;
            if (count == n) {
                return (seq_t)m;
            }
        }
    }
}

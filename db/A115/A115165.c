/*
 * A115165  Odd numbers k such that k-1 and k+1 have the same number of
 *          distinct prime divisors.
 * https://oeis.org/A115165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth odd number k such that k-1 and k+1 have the same
 *            number of distinct prime divisors
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but undefined for n==0 */

    idx_t found = 0;
    idx_t cand = 3;          /* first odd to check */

    while (found < n) {
        uint64_t left  = (uint64_t)(cand - 1);
        uint64_t right = (uint64_t)(cand + 1);

        if (count_distinct_primes(left) == count_distinct_primes(right)) {
            found++;
            if (found == n) {
                return (seq_t)cand;
            }
        }
        cand += 2;   /* move to next odd */
    }

    return 0; /* unreachable */
}

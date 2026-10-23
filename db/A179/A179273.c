/*
 * A179273  Primes in A179272.
 * https://oeis.org/A179273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n*(n-2)/4 - 1) where n starts at 5 and only prime
 *            values of the expression are taken. That is, a(1)=2, a(2)=5,
 *            a(3)=7, … as listed in OEIS A179273
 * Range:     u8 n <= 18, u16 n <= 145, u32 n <= 182, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; undefined for 0 */

    idx_t  count = 0;          /* how many primes found so far */
    seq_t  val;                /* current candidate value */

    /* n >= 1 here; start checking from k=5 */
    for (idx_t k = 5; ; ++k) {
        /* compute the floor of k*(k-2)/4 minus 1, using exact integer arithmetic */
        val = ((seq_t)k * (seq_t)(k - 2)) / 4u - 1;

        if (is_prime_big(val)) {
            ++count;
            if (count == n) return val;
        }
    }
}

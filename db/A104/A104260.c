/*
 * A104260  Sum of odd digits (1,3,5,7,9) of n-th prime.
 * https://oeis.org/A104260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of odd digits of the n‑th prime
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   is_prime_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_idx.h"

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)                           /* undefined, return 0 for safety */
        return (seq_t)0;

    idx_t count   = 0;                     /* primes seen so far */
    idx_t cand    = 2;                     /* current candidate */

    for (;; ++cand) {
        if (!is_prime_idx(cand))
            continue;

        ++count;
        if (count == n) {                 /* found the n‑th prime */
            seq_t sum = 0;
            idx_t tmp = cand;
            while (tmp) {
                int d = tmp % 10;
                if (d & 1)                /* d is odd (1,3,5,7,9) */
                    sum += d;
                tmp /= 10;
            }
            return sum;
        }
    }
}

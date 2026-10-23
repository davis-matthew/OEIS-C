/*
 * A022764  (n-th 8k+3 prime plus n-th 8k+5 prime)/8.
 * https://oeis.org/A022764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (p3(n) + p5(n)) / 8 * where p3(n) is the n-th prime ≡ 3
 *            (mod 8) and p5(n) is the n-th prime ≡ 5 (mod 8)
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (not defined for this sequence) by returning 0 */
    if (n == 0) return (seq_t)0;

    /* We generate primes of the two required residue classes independently.
     * We scan odd numbers starting from 3 and stop when we have collected
     * n primes of each class. */
    idx_t count3 = 0, count5 = 0;
    uint64_t cand  = 3;          /* start with the smallest odd candidate */
    uint64_t p3 = 0, p5 = 0;     /* storage for the n-th primes */

    while (count3 < n || count5 < n) {
        /* Only numbers ≡ 3 or 5 (mod 8) can be the required primes */
        if ((cand & 7) == 3) {           /* cand % 8 == 3 */
            if (is_prime((idx_t)cand)) {
                count3++;
                if (count3 == n) p3 = cand;
            }
        } else if ((cand & 7) == 5) {    /* cand % 8 == 5 */
            if (is_prime((idx_t)cand)) {
                count5++;
                if (count5 == n) p5 = cand;
            }
        }
        cand += 2;   /* skip the even number */
    }

    /* Sum the two primes and divide by 8.  The division is exact */
    seq_t sum = (seq_t)p3 + (seq_t)p5;
    return sum / (seq_t)8;
}

/*
 * A100877  Greater of two consecutive primes of form 4k+3 and 4k+1
 *          respectively.
 * https://oeis.org/A100877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = greater of each consecutive pair of primes (p, q) with
   p ≡ 3 (mod 4) and q ≡ 1 (mod 4).  That is, a(n) is the
   n‑th 1‑mod‑4 prime that is immediately preceded by a 3‑mod‑4 prime.
   Example: a(1)=5, a(2)=13, a(3)=29, ...
*/

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* n is guaranteed ≥1 by the sequence offset */
    idx_t count = 0;
    uint64_t prev_prime = 2;  /* previous prime encountered */

    for (uint64_t cand = 3; ; cand += 2) {        /* skip even numbers */
        if (is_prime(cand)) {
            if (((cand   ) & 3ULL) == 1ULL &&   /* cand ≡ 1 (mod 4)   */
                ((prev_prime) & 3ULL) == 3ULL) { /* prev_prime ≡ 3     */
                count++;
                if (count == n) {
                    return (seq_t)cand;         /* found the n-th term */
                }
            }
            prev_prime = cand;
        }
    }
}

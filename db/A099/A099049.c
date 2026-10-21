/*
 * A099049  Numbers n such that n-1 and n+1 are both prime or both composite.
 * https://oeis.org/A099049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   isPrimeIdx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) = the nth integer n such that n-1 and n+1 are both prime
   or both composite.  Computation is done by a simple brute‑force search. */

#include <stdbool.h>
#include "../utils/isPrimeIdx.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t x = 4;                     /* first term is 4 */
    while (1) {
        idx_t a_minus = x - 1;
        idx_t a_plus  = x + 1;
        bool a_prime = isPrimeIdx(a_minus);
        bool b_prime = isPrimeIdx(a_plus);
        if (a_prime == b_prime) {   /* both prime or both composite */
            count++;
            if (count == n) {
                return (seq_t)x;
            }
        }
        x++;
    }
}

/*
 * A175553  Product of first k triangular numbers divided by the sum of first
 *          k triangular numbers is an integer.
 * https://oeis.org/A175553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175553
   a(n) = the n-th integer k such that
          Π_{i=1..k} (i*(i+1)/2)  /  Σ_{i=1..k} (i*(i+1)/2)  is an integer.
   For k > 6 this holds iff k+2 is composite; for k ≤ 6 only k = 1 and k = 4. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)4;

    idx_t count = 2;          /* already accounted for 1 and 4 */
    for (idx_t k = (idx_t)7; ; ++k) {
        idx_t val = k + (idx_t)2;
        if (!is_prime(val)) {    /* composite: k qualifies */
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}

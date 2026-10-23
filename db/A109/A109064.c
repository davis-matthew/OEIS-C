/*
 * A109064  Expansion of eta(q)^5 / eta(q^5) in powers of q.
 * https://oeis.org/A109064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 28, i16 n <= 6060, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   b_of_n, kronecker_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A109064
   Formula: a(n) = -5 * b(n)  for n > 0, a(0)=1
   where b is multiplicative with
   b(1)=1,   b(5^e)=1,
   and for an odd prime p≠5 (p=2 allowed) and p^e (e≥1):
          b(p)   =  ( Kronecker(5,p) ==  1 ) ? (p+1) : -(p-1)
          b(p^e) =  b(p) * b(p^(e-1))  -  Kronecker(5,p) * p * b(p^(e-2))
*/

#include <stdint.h>
#include "../utils/b_of_n.h"
#include "../utils/kronecker_5.h"

typedef uint64_t idx_t;   // index type supplied by the build
/* seq_t is defined by the build; do not redefine it here */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;     /* a(0)=1 */
    seq_t bval = b_of_n(n);
    return - (seq_t)5 * bval;
}

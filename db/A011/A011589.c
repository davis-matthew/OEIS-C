/*
 * A011589  Legendre symbol (n,37).
 * https://oeis.org/A011589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A011589: Legendre symbol (n,37)
 *  This implements the Dirichlet character of the field Q(√37).
 *  For p = 37 (prime) the Legendre symbol (a|p) satisfies
 *      (a|p) =  0   if p | a
 *             =  1   if a^( (p-1)/2 ) ≡ 1 (mod p)
 *             = -1   otherwise
 *  Therefore for all n ≥ 0:
 *      a(0) = 0
 *      a(n) = Legendre symbol (n,37)
 *
 *  Terms: a(0)=0, a(1)=1, a(2)=-1, a(3)=1, ...
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Quick path for n == 0 */
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    /* Reduce n modulo 37 */
    uint64_t r = n % 37ull;          /* r in 0..36 */
    if (r == 0ull) {
        return (seq_t)0;
    }

    /* Compute r^18 mod 37 by repeated multiplication */
    seq_t base = (seq_t)r;
    seq_t result = (seq_t)1;
    /* 18 is (37-1)/2 */
    for (idx_t i = 0; i < 18; ++i) {
        result = (result * base) % 37;
    }

    /* Legendre symbol: 1 if result == 1, else -1 */
    if (result == (seq_t)1) {
        return (seq_t)1;
    } else {
        return (seq_t)-1;
    }
}

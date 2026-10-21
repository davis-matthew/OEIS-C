/*
 * A081547  Smallest composite number which is 1 more than the product of n
 *          (not necessarily distinct) prime numbers.
 * https://oeis.org/A081547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=10, for n>2, a(n)=2^n+1 unless n∈{4,8,16} (known
 *            non‑trivial Fermat exponents), in which case a(n)=2^(n‑1)*3+1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special small cases */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;

    /* Check for the only known Fermat exponents that give a Fermat prime
       2^n+1 for n>2 : 4, 8, 16.  For other n (including larger powers of 2)
       2^n+1 is composite. */
    if ((n & (n - 1)) == 0 && (n == 4 || n == 8 || n == 16)) {
        seq_t p = big_pow((seq_t)2, (seq_t)(n - 1)); /* 2^(n-1) */
        p = p * (seq_t)3;     /* multiply by 3 */
        return p + (seq_t)1; /* +1 */
    }

    /* Generic case: a(n) = 2^n + 1 */
    seq_t p = big_pow((seq_t)2, (seq_t)n);         /* 2^n */
    return p + (seq_t)1;                           /* +1 */
}

/*
 * A127553  a(n) = Product_{k=1..n} lcm(k,n)/gcd(k,n).
 * https://oeis.org/A127553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} lcm(k,n)/gcd(k,n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Computes the OEIS sequence A127553.  The formula can be
   simplified to (k/g)*(n/g) for each term of the product,
   where g = gcd(k,n).  The implementation uses only the
   arithmetic operators allowed for seq_t, and the provided
   gcd function. */

seq_t a(idx_t n)
{
    /* The product over an empty range is defined to be 1,
       which also works for n == 0 (offset 1 in the OEIS). */
    seq_t result = (seq_t)1;
    seq_t seq_n  = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t seq_k = (seq_t)k;
        seq_t g = gcd(seq_n, seq_k);           /* g = gcd(k,n) */
        seq_t term = (seq_k / g) * (seq_n / g);/* (k/g)*(n/g) */
        result *= term;
    }

    return result;
}

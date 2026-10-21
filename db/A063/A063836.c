/*
 * A063836  Numbers k such that usigma(sigma(k)) is prime.
 * https://oeis.org/A063836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, bref
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063836
   Numbers k such that sigma(k)+1 is a Fermat prime.
   Equivalently, k is a product of distinct Mersenne primes
   (p in the list of exponents of Mersenne primes) whose
   exponents sum to a power of two s for which 2^(2^s)+1
   (a Fermat prime) is known.
   Since only the first five Fermat primes are known, there
   are only three such k:
       s=1  → exponents {2}          → k=3
       s=3  → exponents {3,5}        → k=217
       s=4  → exponents {3,13}       → k=57337
   The sequence is finite; for n>3 we return 0.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Only three terms are known.  */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)217;
    if (n == 3) return (seq_t)57337;
    return 0;                  /* undefined terms */
}

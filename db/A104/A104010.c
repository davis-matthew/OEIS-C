/*
 * A104010  Sum of two successive sexy primes.
 * https://oeis.org/A104010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A104010 (Sum of two successive sexy primes)
 * a(n) = 2*p + 6  where p is the smaller prime of the n-th sexy prime pair
 * (p, p+6).
 * The function searches for successive primes p such that both p and p+6 are
 * prime and returns the sum when the n-th pair is reached.
 */
#include "../utils/is_prime.h"
#include <stdint.h>          /* for uint64_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, but harmless */

    idx_t count = 0;
    uint64_t p = 5;                 /* first possible p (odd primes ≥5) */

    while (1) {
        if (is_prime(p) && is_prime(p + 6)) {
            ++count;
            if (count == n) {
                return (seq_t)2 * (seq_t)p + (seq_t)6;
            }
        }
        p += 2;                     /* skip even numbers */
    }
}

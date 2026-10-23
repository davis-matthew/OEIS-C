/*
 * A122984  Smallest positive composite number, relatively prime to n, such
 *          that a(n)+n is also composite.
 * https://oeis.org/A122984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest positive composite number, relatively prime to n,
   such that a(n)+n is also composite. */
/* This implementation performs a straightforward search
   starting from the first composite number (4).  For each candidate m,
   it checks that gcd(m,n)=1, that m is composite (i.e. not prime),
   and that m+n is also composite.  The first candidate satisfying all
   conditions is returned. */
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t m = 4; /* first composite number */
    while (1) {
        /* Require m and n to be coprime */
        if (gcd(m, n_seq) == 1) {
            /* m must be composite: not prime */
            if (!is_prime_big(m) && !is_prime_big(m + n_seq)) {
                return m;
            }
        }
        ++m;
    }
}

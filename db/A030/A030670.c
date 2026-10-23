/*
 * A030670  Smallest prime formed by appending a number to the n-th prime.
 * https://oeis.org/A030670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime obtained by appending a positive integer
 *            (whose first digit is non‑zero) to the n‑th prime
 * Range:     u8 n <= 9, u16 n <= 126, u32 n <= 102, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, get_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Get the n‑th prime (1‑indexed). */
    seq_t p = get_prime(n);

    /* Try increasing integers k = 1, 2, 3, … until the concatenation
       p || k is prime.  */
    for (idx_t k = 1;; ++k)
    {
        /* Number of decimal digits of k.  */
        idx_t d = digits(k);

        /* 10^d.  */
        seq_t pow = pow10(d);

        /* Concatenation: p * 10^d + k.  */
        seq_t cand = p * pow + (seq_t)k;

        /* Test primality.  */
        if (is_prime_big(cand))
            return cand;
    }
}

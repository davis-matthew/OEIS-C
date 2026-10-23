/*
 * A178220  Smallest number that appending to n-th prime gives another prime.
 * https://oeis.org/A178220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest positive integer x such that concatenating the
 *            n-th prime pₙ with x yields a prime
 * Range:     u8 n <= 10, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   digits, get_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Get the n-th prime (1‑based).  The required auxiliary function
       get_prime is provided by the build system. */
    seq_t p = get_prime(n);

    /* Search for the smallest suffix x (starting from 1) that makes
       p * 10^digits(x) + x a prime.  The `digits` function returns the
       number of decimal digits of its argument, and `pow10` returns
       10^d as a seq_t value.  The function `is_prime_big` can handle
       arbitrary‑precision numbers. */
    idx_t x = 1;
    while (1) {
        idx_t d = (idx_t)digits(x);
        seq_t base = pow10(d);
        seq_t candidate = p * base + x;
        if (is_prime_big(candidate))
            return (seq_t)x;
        x++;
    }
}

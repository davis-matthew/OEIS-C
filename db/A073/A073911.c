/*
 * A073911  Smallest number m such that m and the product of digits of m are
 *          both divisible by 5n, or 0 if no such number exists.
 * https://oeis.org/A073911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A085124(5*n) // smallest integer m such that m and the
 *            product of its digits are both divisible by 5n; 0 if no such m
 *            exists
 * Range:     u8 n <= 4, u16 n <= 74, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    /* 0 is not considered divisible by any number. */
    /* If n is even, 5n contains the factor 2 -> impossible. */
    if (n % 2 == 0)
        return 0;

    /* Factor n by the allowed primes 3, 5, 7 only. */
    idx_t t = n;
    while (t % 3 == 0) t /= 3;
    while (t % 5 == 0) t /= 5;
    while (t % 7 == 0) t /= 7;
    /* If any prime factor > 9 remains, the product of digits can never contain it. */
    if (t != 1)
        return 0;

    /* D = 5n */
    seq_t D = ((seq_t)5) * ((seq_t)n);

    /* Brute‑force multiples of D until the digit product is divisible by D. */
    for (idx_t k = 1; ; ++k) {
        seq_t m = D * ((seq_t)k);
        seq_t p = digit_product(m);
        if (p != 0 && p % D == 0)
            return m;
    }
}

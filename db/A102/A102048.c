/*
 * A102048  Exponent of A046021(n) (least inverse of Kempner function A002034)
 *          when written as a power of A006530(n) (largest prime dividing n),
 *          with a(1) = 1.
 * https://oeis.org/A102048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..floor(log(n-1)/log(P))} floor((n-1)/P^k)
 *            with P = largest_prime_factor(n). a(1) = 1
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   greatest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/greatest_prime_factor.h"

seq_t a(idx_t n) {
    if (n == 1)
        return 1;
    seq_t x = (seq_t)n;
    seq_t P = greatest_prime_factor(x);   /* largest prime factor of n */
    seq_t vmax = x - 1;
    seq_t power = P;
    seq_t sum = 0;
    while (power <= vmax) {
        sum += vmax / power;
        /* prevent overflow when multiplying power by P */
        if (power > vmax / P)
            break;
        power *= P;
    }
    return sum + 1;
}

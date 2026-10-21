/*
 * A073819  a(n) = prime(i) such that prime(i)*(n+1-i) is maximized (1 <= i <=
 *          n).
 * https://oeis.org/A073819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(i) such that prime(i)*(n + 1 − i) is maximized for
 *            1 ≤ i ≤ n
 * Range:     u8 n <= 17, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product is maximized; in case of a tie the smallest i (hence smallest prime) is chosen. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is at least 1 by definition of the sequence. */
    idx_t i;
    seq_t best_prime = 0;
    seq_t best_prod  = 0;

    for (i = 1; i <= n; ++i) {
        seq_t p   = (seq_t)get_prime(i);                /* prime(i) */
        seq_t prod = p * ((seq_t)(n + 1 - i));          /* product with decreasing multiplier */
        if (prod > best_prod) {
            best_prod  = prod;
            best_prime = p;
        }
    }
    return best_prime;
}

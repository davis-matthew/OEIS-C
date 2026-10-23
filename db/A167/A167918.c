/*
 * A167918  a(n) is smallest index k > n of k-th prime with
 *          f(n,k):=(p(k)+p(k+1))/(p(n)+p(n+1)) an integer >=2 (n=1,2,...).
 * https://oeis.org/A167918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > n such that (p(k)+p(k+1)) / (p(n)+p(n+1)) is
 *            an integer >= 2
 * Range:     u8 n <= 8, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required external functions provided by the build environment */
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* denominator for the ratio */
    seq_t denom = (seq_t)get_prime(n) + (seq_t)get_prime(n + 1);
    if (denom == 0) return 0; /* safety, should not happen */

    idx_t k = n + 1;
    while (1) {
        seq_t pk   = (seq_t)get_prime(k);
        seq_t pk1  = (seq_t)get_prime(k + 1);
        seq_t num  = pk + pk1;

        /* Check divisibility and that the quotient is at least 2 */
        if (num % denom == 0) {
            seq_t ratio = num / denom;
            if (ratio >= (seq_t)2)
                return (seq_t)k;
        }
        ++k;
    }
}

/*
 * A062785  a(n) = Chowla's function of n * sigma(n).
 * https://oeis.org/A062785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (sigma(n) - n - 1) * sigma(n), with sigma(1) = 1 and a(1)
 *            = 0. Here sigma(n) is the sum of all positive divisors of n
 * Range:     u8 n <= 11, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1)
        return 0;                       /* sigma(1)=1, but a(1)=0 by definition */

    /* Compute sigma(n) via prime factorisation */
    seq_t sigma = 1;
    idx_t t = n;

    /* handle factor 2 separately for a bit of speed */
    if ((t & 1) == 0) {
        seq_t term = 1;
        seq_t power = 1;
        while ((t & 1) == 0) {
            t >>= 1;
            power *= (seq_t)2;
            term += power;
        }
        sigma *= term;
    }

    /* trial division by odd numbers */
    for (idx_t i = 3; i * i <= t; i += 2) {
        if (t % i == 0) {
            seq_t term = 1;
            seq_t power = 1;
            while (t % i == 0) {
                t /= i;
                power *= (seq_t)i;
                term += power;
            }
            sigma *= term;
        }
    }

    /* remaining prime factor larger than sqrt(original n) */
    if (t > 1) {
        sigma *= ((seq_t)1 + (seq_t)t);
    }

    /* a(n) = sigma * (sigma - n - 1) */
    seq_t diff = sigma - (seq_t)n - (seq_t)1;
    return sigma * diff;
}

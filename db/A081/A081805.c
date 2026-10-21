/*
 * A081805  a(n) = n minus (largest prime power in n factorization); a(1) = 0.
 * https://oeis.org/A081805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (largest prime power in the factorization of n).
 *            (A081805: a(1)=0, a(n)=0 when n is a prime power.)
 * Range:     u8 n <= 272, u16 n <= 9144, u32 n <= 9144, u64 n <= 9144,
 *            u128 n <= 9144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute a(n).  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;   /* a(1) = 0, and nothing below 1 is defined */

    idx_t temp = n;
    seq_t largest = (seq_t)1;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p != 0)
            continue;

        seq_t power = (seq_t)1;
        while (temp % p == 0) {
            temp /= p;
            power *= (seq_t)p;
        }
        if (power > largest)
            largest = power;
    }

    /* If a prime factor remains (temp > 1), it's the largest prime factor. */
    if (temp > 1 && (seq_t)temp > largest)
        largest = (seq_t)temp;

    return (seq_t)n - largest;
}

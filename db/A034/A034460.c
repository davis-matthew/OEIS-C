/*
 * A034460  a(n) = usigma(n) - n, where usigma(n) = sum of unitary divisors of
 *          n (A034448).
 * https://oeis.org/A034460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = usigma(n) – n, where usigma(n) = ∏(1 + p^e) over the
 *            prime factorization n = ∏p^e
 * Range:     u8 n <= 209, u16 n <= 9945, u32 n <= 9945, u64 n <= 9945,
 *            u128 n <= 9945, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; undefined values are irrelevant */
        return 0;

    seq_t prod = 1;            /* product of (1 + p^e) */
    idx_t temp = n;

    /* treat factor 2 separately (faster) */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            e++;
        }
        seq_t pow = 1;
        for (idx_t i = 0; i < e; ++i)
            pow *= (seq_t)2;
        prod *= (pow + 1);
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            seq_t pow = 1;
            for (idx_t i = 0; i < e; ++i)
                pow *= (seq_t)p;
            prod *= (pow + 1);
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
        prod *= ((seq_t)temp + 1);

    /* a(n) = usigma(n) – n */
    return prod - (seq_t)n;
}

/*
 * A033884  Deficiency of n-th deficient number: 2k - sigma(k) for k such that
 *          this is > 0.
 * https://oeis.org/A033884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2k - sigma(k) for the n‑th deficient integer k *
 *            (deficient iff 2k - sigma(k) > 0)
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the sum of positive divisors sigma(n) */
static seq_t sigma_of(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defensive; never used */

    uint64_t temp = (uint64_t)n;          /* factorization done in 64‑bit */
    seq_t result = (seq_t)1;              /* start with divisor 1 */

    /* factor 2 */
    if ((temp & 1U) == 0) {
        seq_t sumpow = (seq_t)1;          /* 1 + 2 + 2^2 + … */
        seq_t pow    = (seq_t)1;
        while ((temp & 1U) == 0) {
            temp >>= 1;
            pow   = pow * (seq_t)2;
            sumpow = sumpow + pow;
        }
        result = result * sumpow;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t sumpow = (seq_t)1;
            seq_t pow    = (seq_t)1;
            while (temp % p == 0) {
                temp = temp / p;
                pow  = pow * (seq_t)p;
                sumpow = sumpow + pow;
            }
            result = result * sumpow;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        result = result * ((seq_t)1 + (seq_t)temp);
    }

    return result;
}

seq_t a(idx_t n)
{
    idx_t count = 0;   /* how many deficient numbers have been seen */
    for (idx_t k = 1; ; ++k) {
        seq_t sigma = sigma_of(k);
        seq_t twok  = ((seq_t)k << 1);   /* 2k */

        if (sigma < twok) {              /* k is deficient */
            ++count;
            if (count == n) {
                return twok - sigma;
            }
        }
    }
    /* never reached for n >= 1 */
    return (seq_t)0;
}

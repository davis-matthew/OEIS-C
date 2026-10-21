/*
 * A068327  Arithmetic derivative of n^n.
 * https://oeis.org/A068327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n^n * sum_{p|n} (n/p) (arithmetic derivative of n^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute base^exp in seq_t using binary exponentiation */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1u)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle special cases 0 and 1 */
    if (n == 0 || n == 1)
        return 0;

    /* Compute sum = Σ_{p|n} (n / p) with multiplicities */
    seq_t sum = 0;
    idx_t temp = n;
    /* Check factor 2 separately to allow odd increments */
    if (temp % 2 == 0) {
        idx_t e = 0;
        while (temp % 2 == 0) {
            e++;
            temp /= 2;
        }
        sum += (seq_t)(e * (n / 2));
    }
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            sum += (seq_t)(e * (n / p));
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        sum += (seq_t)(n / temp);
    }

    /* Compute n^n */
    seq_t nseq = (seq_t)n;
    seq_t power = pow_seq(nseq, n);

    /* a(n) = n^n * sum */
    return power * sum;
}

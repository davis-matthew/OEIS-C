/*
 * A228274  a(n) = Sum_{d|n, n/d odd} n * d.
 * https://oeis.org/A228274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * 2^e * Sum_{d|m,d odd} d, where m = n / 2^e and e is
 *            the exponent of 2 in n
 * Range:     u8 n <= 14, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>


/* Computes A228274 for index n (n >= 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* not needed for the OEIS offset */

    /* Extract the 2‑adic exponent e and the odd part m. */
    idx_t temp = n;
    idx_t e = 0;
    while ((temp & 1ULL) == 0) {             /* while divisible by 2 */
        temp >>= 1;
        ++e;
    }
    idx_t m = temp;                          /* now m is odd */

    /* 2^e as a seq_t. */
    seq_t two_pow_e = 1;
    if (e > 0) two_pow_e <<= e;              /* shift uses seq_t */

    /* Product of sums of divisors for all odd prime powers of m. */
    seq_t sum_odd_divisors = 1;

    idx_t remainder = m;                     /* factor remainder */
    for (idx_t p = 3; (p * p) <= remainder; p += 2) {
        if (remainder % p == 0) {
            idx_t exp = 0;
            while (remainder % p == 0) {
                remainder /= p;
                ++exp;
            }
            /* Compute (p^(exp+1) - 1) / (p - 1) as a seq_t. */
            seq_t p_pow = 1;
            for (idx_t i = 0; i < exp + 1; ++i) {
                p_pow *= (seq_t)p;
            }
            seq_t sum_div = (p_pow - 1) / ((seq_t)p - 1);
            sum_odd_divisors *= sum_div;
        }
    }
    if (remainder > 1) {                     /* remainder now a prime > 2 */
        seq_t sum_div = 1 + (seq_t)remainder;
        sum_odd_divisors *= sum_div;
    }

    seq_t n_seq = (seq_t)n;
    return n_seq * two_pow_e * sum_odd_divisors;
}

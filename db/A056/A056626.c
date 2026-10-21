/*
 * A056626  Number of non-unitary square divisors of n.
 * https://oeis.org/A056626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046951(n) - 2^r(n) where A046951(n) is the number of
 *            square divisors of n and r(n) is the number of distinct prime
 *            factors of the largest unitary square divisor of n. For a prime
 *            power n = p^u, a(p^u) = u - 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0, return 0 */

    uint64_t m = (uint64_t)n;          /* working copy of n */

    seq_t square_div_cnt = 1;          /* count of square divisors */
    unsigned even_exp_primes = 0;      /* number of primes with even exponent */

    /* handle factor 2 separately */
    uint64_t e = 0;
    while ((m & 1) == 0) {             /* m % 2 == 0 */
        e++;
        m >>= 1;
    }
    if (e) {
        square_div_cnt *= (seq_t)(e / 2 + 1);
        if ((e & 1) == 0)
            even_exp_primes++;
    }

    /* trial division with odd numbers */
    for (uint64_t p = 3; p <= m / p; p += 2) {   /* p*p <= m */
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            square_div_cnt *= (seq_t)(e / 2 + 1);
            if ((e & 1) == 0)
                even_exp_primes++;
        }
    }

    /* remaining factor (prime > sqrt original n) */
    if (m > 1) {
        /* exponent is 1, so contributes factor 1 to square_div_cnt */
        if (1 % 2 == 0)    /* 1 is odd, so no increment of even_exp_primes */
            even_exp_primes++;   /* unreachable but kept for completeness */
    }

    /* number of unitary square divisors = 2^even_exp_primes */
    seq_t unitary_square_cnt = (seq_t)1 << even_exp_primes;

    return square_div_cnt - unitary_square_cnt;
}

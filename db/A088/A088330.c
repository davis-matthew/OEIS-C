/*
 * A088330  Sum of the remainders when n is divided by nonzero numbers
 *          obtained by deleting one digit. The sum ranges over all the
 *          digits.
 * https://oeis.org/A088330
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = sum over each position of the remainder when n is divided
 *            by the number obtained by deleting that digit, ignoring
 *            deletions that produce 0
 * Range:     u8 n <= 458, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned long long pow10[21] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

seq_t a(idx_t n)
{
    /* Convert n to seq_t for modulus operations */
    seq_t n_seq = (seq_t)n;

    /* Determine number of decimal digits */
    unsigned d = 0;
    unsigned long long temp = n;
    while (temp) {
        d++;
        temp /= 10;
    }

    seq_t sum = 0;
    for (unsigned i = 0; i < d; ++i) {
        unsigned pow_high = d - i;             /* 10^(d-i)   */
        unsigned pow_low  = d - i - 1;         /* 10^(d-i-1) */
        unsigned long long prefix = n / pow10[pow_high];
        unsigned long long suffix = n % pow10[pow_low];
        unsigned long long m_raw = prefix * pow10[pow_low] + suffix;
        if (m_raw == 0) continue;              /* skip non‑zero requirement */
        seq_t m = (seq_t)m_raw;
        sum += n_seq % m;
    }
    return sum;
}

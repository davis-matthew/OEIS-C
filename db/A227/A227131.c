/*
 * A227131  Sum of divisors of n that are not divisible by 25. a(0) = 1.
 * https://oeis.org/A227131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum_{d|n, 25∤d} d (a(0)=1)
 * Range:     u8 n <= 107, u16 n <= 9982, u32 n <= 9982, u64 n <= 9982,
 *            u128 n <= 9982, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* static list of primes up to 100 (√10000) */
static const idx_t _primes_upto_100[] = {
    2, 3, 5, 7,
    11, 13, 17, 19,
    23, 29, 31, 37,
    41, 43, 47, 53,
    59, 61, 67, 71,
    73, 79, 83, 89,
    97
};

/*
 * Compute σ(n) = sum of all positive divisors of n.
 * For n = 0 a(0) = 1 is defined separately.
 */
static seq_t _sigma_u64(idx_t n)
{
    if (n == 0ull) return (seq_t)1;   /* used only when n=0 */

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Iterate over primes up to √temp. */
    for (size_t i = 0; i < sizeof _primes_upto_100 / sizeof _primes_upto_100[0]; ++i) {
        idx_t p = _primes_upto_100[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            seq_t term = (seq_t)1;      /* term = 1 + p + p^2 + … + p^e */
            seq_t power = (seq_t)1;     /* power = p^k */
            while (temp % p == 0) {
                temp /= p;
                power *= (seq_t)p;
                term += power;
            }
            result *= term;
        }
    }

    /* If a prime factor > √temp remains. */
    if (temp > 1ull) {
        /* term = 1 + temp */
        result *= (seq_t)(temp + 1ull);
    }

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0ull) return (seq_t)1;

    seq_t total = _sigma_u64(n);

    if (n % 25ull == 0ull) {
        seq_t sub = _sigma_u64(n / 25ull);
        total -= (seq_t)25ull * sub;
    }

    return total;
}

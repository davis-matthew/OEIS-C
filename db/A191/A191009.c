/*
 * A191009  The remainder of (sum of proper divisors of n) mod (largest proper
 *          divisor of n).
 * https://oeis.org/A191009
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001065(n) mod A032742(n)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A(n) is the sum of proper divisors of n modulo the largest proper divisor of n  */
#include <stdint.h>

/* Helper: find smallest prime factor of n (n >= 2).  */
static uint64_t _spf(uint64_t n) {
    if ((n & 1ULL) == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return d;
    }
    return n; /* n itself is prime */
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* sequence is undefined below 2 */

    /* Largest proper divisor = n / smallest prime factor */
    uint64_t spf = _spf((uint64_t)n);
    seq_t largest = (seq_t)n / (seq_t)spf;

    /* Compute σ(n) i.e. sum of all divisors of n */
    uint64_t temp = (uint64_t)n;
    seq_t sigma = 1;

    while (temp > 1) {
        /* factor = smallest prime divisor of the remaining part */
        uint64_t p = _spf(temp);
        /* count the exponent of p */
        uint64_t k = 0;
        while (temp % p == 0) {
            temp /= p;
            k++;
        }

        /* term = 1 + p + p^2 + ... + p^k  (k >= 1) */
        seq_t term = 1;
        seq_t pk = 1;
        for (uint64_t i = 0; i < k; ++i) {
            pk *= (seq_t)p;
            term += pk;
        }

        sigma *= term;
    }

    /* Sum of proper divisors = σ(n) – n */
    seq_t proper = sigma - (seq_t)n;

    /* Result: proper modulo largest proper divisor */
    return proper % largest;
}

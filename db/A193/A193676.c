/*
 * A193676  Number of nonnegative zeros of minimal polynomials of 2*cos(Pi/n),
 *          n>=1.
 * https://oeis.org/A193676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 1 ≤ k ≤ ⌊n/2⌋ with gcd(k,2n)=1 Equivalent to
 *            counting odd k ≤ ⌊n/2⌋ that are coprime to n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* m = ⌊n/2⌋ */
    idx_t m = n >> 1;

    /* collect odd prime factors of n */
    idx_t primes[64];
    size_t pc = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (!(temp % p)) {
            if (p != 2)            /* 2 is ignored because k must be odd */
                primes[pc++] = p;
            while (!(temp % p))
                temp /= p;
        }
    }
    if (temp > 1 && temp != 2)   /* remaining prime factor > sqrt(n) */
        primes[pc++] = temp;

    /* Count odd numbers ≤ m */
    long long ans = (long long)((m + 1) >> 1);   /* total odd k */

    /* Inclusion–exclusion over odd prime factors of n */
    if (pc > 0) {
        size_t subsets = 1ULL << pc;        /* 2^pc subsets */
        for (size_t mask = 1; mask < subsets; ++mask) {
            idx_t d = 1;
            int bits = 0;
            for (size_t j = 0; j < pc; ++j) {
                if (mask & (1ULL << j)) {
                    ++bits;
                    d *= primes[j];
                }
            }
            idx_t R = m / d;                 /* floor(m/d) */
            long long cnt = (long long)((R + 1) >> 1);  /* odd multiples of d */

            if (bits & 1)
                ans -= cnt;
            else
                ans += cnt;
        }
    }

    return (seq_t)ans;
}

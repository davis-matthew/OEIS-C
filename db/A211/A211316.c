/*
 * A211316  Maximal size of sum-free set in additive group of integers mod n.
 * https://oeis.org/A211316
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(p+1)/(3p) where p is the smallest prime divisor of n
 *            with p≡2 (mod 3); otherwise if n%3==0 a(n)=n/3; otherwise
 *            a(n)=(n-1)/3
 * Range:     u8 n <= 16, u16 n <= 7830, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) {                     /* sequence undefined before the offset */
        return 0;
    }

    /* Find the smallest prime divisor p of n with p ≡ 2 (mod 3). */
    uint64_t p = 0;
    uint64_t temp = (uint64_t)n;

    for (uint64_t i = 2; i <= temp / i; ++i) {
        if (temp % i == 0) {
            if (i % 3 == 2) {        /* found smallest divisor ≡ 2 mod 3 */
                p = i;
                break;
            }
            /* Remove all copies of this divisor. */
            do {
                temp /= i;
            } while (temp % i == 0);
        }
    }

    /* If no divisor ≡ 2 mod 3 was found during trial division,
       the remaining factor (if any) is prime. */
    if (p == 0 && temp > 1 && temp % 3 == 2) {
        p = temp;
    }

    seq_t N = (seq_t)n;

    if (p != 0) {                    /* rule for a prime divisor ≡ 2 mod 3 */
        seq_t num = N * (((seq_t)p) + 1);
        seq_t den = (seq_t)3 * (seq_t)p;
        return num / den;
    } else if (n % 3 == 0) {         /* divisible by 3 but no such prime divisor */
        return N / 3;
    } else {                         /* all prime divisors ≡ 1 mod 3 */
        return (N - 1) / 3;
    }
}

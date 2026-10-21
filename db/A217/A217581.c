/*
 * A217581  Largest prime divisor of n <= sqrt(n), 1 if n is prime or 1.
 * https://oeis.org/A217581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the greatest prime divisor d of n such that d ≤ √n; if
 *            no such divisor exists (i.e., n is prime or 1), a(n) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;

    /* compute floor of integer sqrt of n */
    uint64_t sqrt_n = 0;
    for (uint64_t i = 1; i * i <= nn; ++i)
        sqrt_n = i;

    uint64_t best = 1;
    for (uint64_t d = 2; d <= sqrt_n; ++d) {
        if (nn % d != 0)
            continue;

        /* check if d is prime */
        bool is_prime = true;
        for (uint64_t p = 2; p * p <= d; ++p) {
            if (d % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime && d > best)
            best = d;
    }

    return (seq_t)best;
}

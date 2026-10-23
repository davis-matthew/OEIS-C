/*
 * A182481  a(n) is the least k such that 6*k*n-1 and 6*k*n+1 are twin primes,
 *          and a(n)=0, if such k does not exist.
 * https://oeis.org/A182481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k such that 6*k*n-1 and 6*k*n+1 are twin
 *            primes; return 0 if no such k exists (the conjecture predicts
 *            this never happens)
 * Range:     u8 n <= 12, u16 n <= 2130, u32 n <= 2130, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    /* Search for the smallest k >= 1 */
    for (uint64_t k = 1; ; ++k) {
        /* Compute 6*k*n using a wide type to avoid intermediate overflow */
        unsigned __int128 base = (unsigned __int128)6 * k * n;
        seq_t p1 = (seq_t)(base - 1);     /* 6*k*n - 1 */
        seq_t p2 = (seq_t)(base + 1);     /* 6*k*n + 1 */

        /* Check primality of both numbers */
        if (is_prime_big(p1) && is_prime_big(p2)) {
            return (seq_t)k;
        }
    }
}

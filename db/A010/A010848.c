/*
 * A010848  Number of numbers k <= n such that at least one prime factor of n
 *          is not a prime factor of k.
 * https://oeis.org/A010848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(n / rad(n)) where rad(n) = product of distinct
 *            prime factors of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes a(n) for OEIS sequence A010848.
   The function accepts any idx_t n (>=1) and returns the value
   as seq_t. It handles large n safely by performing all
   arithmetic in idx_t and casts the final result to seq_t. */
seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* a(1) = 0 as given */

    idx_t rad = 1;
    idx_t temp = n;

    /* Trial‑division to collect distinct prime factors. */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            rad *= p;               /* include distinct prime once */
            while (temp % p == 0)
                temp /= p;           /* remove all powers of p */
        }
    }

    /* Remaining factor > 1 is a distinct prime. */
    if (temp > 1)
        rad *= temp;

    idx_t div = n / rad;         /* floor(n / rad) */
    idx_t ans = n - div;         /* a(n) = n - floor(n / rad) */

    return (seq_t)ans;
}

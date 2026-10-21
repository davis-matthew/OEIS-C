/*
 * A093803  Greatest odd proper divisor of n; a(1)=1.
 * https://oeis.org/A093803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n ≥ 1, a(1)=1; if n is a power of two, a(n)=1; else if n is
 *            even, a(n)=odd part of n (remove all 2s); else (n odd) a(n)=n /
 *            spf(n) where spf(n) is the smallest prime divisor >1
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Remove all factors of 2 (odd part). */
    idx_t odd = n;
    while ((odd & 1) == 0) odd >>= 1;

    /* If odd part is 1, n was a power of two. */
    if (odd == 1) return (seq_t)1;

    /* Even n that is not a power of two: odd part is the answer. */
    if ((n & 1) == 0) return (seq_t)odd;

    /* n is odd and > 1: greatest odd proper divisor is n divided
       by its smallest odd divisor (i.e., by spf(n)). */
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) return (seq_t)(n / p);
    }
    /* n is prime. */
    return (seq_t)1;
}

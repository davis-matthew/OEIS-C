/*
 * A028842  Numbers whose product of digits is prime.
 * https://oeis.org/A028842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 59, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose product of digits is prime (OEIS A028842) */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1, harmless */

    /* Find the length L of the number, i.e. the smallest L with
       2*L*(L+1) >= n.  The cumulative count of terms up to length L is
       2*L*(L+1). */
    idx_t L = 1;
    while (2 * L * (L + 1) < n) ++L;

    /* Count of terms before length L */
    idx_t prev = 2 * (L - 1) * L;

    /* Zero‑based offset inside the block of length L */
    idx_t offset = n - prev - 1;
    idx_t block   = offset / 4;               /* 0 .. L-1, grouped by position */
    idx_t pos     = L - 1 - block;            /* position of the prime digit
                                                (0 = most significant) */
    int  primeIdx = offset % 4;               /* 0..3 selects 2,3,5,7 */

    const int primes[4] = { 2, 3, 5, 7 };

    /* Build the number digit by digit.  All digits are 1 except one
       digit equal to primes[primeIdx] at position 'pos'. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        val = val * (seq_t)10 + (i == pos ? (seq_t)primes[primeIdx] : (seq_t)1);
    }

    return val;
}

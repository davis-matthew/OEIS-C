/*
 * A127944  Partial sums of A093049.
 * https://oeis.org/A127944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)/2 - 2*n + popcount(n)
 * Range:     u8 n <= 24, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0), a(1), a(2) are 0 by definition */
    if (n <= 2) {
        return (seq_t)0;
    }

    /* triangular number n*(n+1)/2 */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* difference tri - 2*n, always non‑negative for n>=3 */
    seq_t diff = tri - (seq_t)2 * n;

    /* popcount of n (number of set bits) */
    seq_t pc = (seq_t)__builtin_popcountll((unsigned long long)n);

    return diff + pc;
}

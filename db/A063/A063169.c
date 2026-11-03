/*
 * A063169  a(n) = n*A001865(n).
 * https://oeis.org/A063169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=0..n-1} n^k * n! / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 150 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: multiply then divide, using 128‑bit intermediate when the
   underlying type is 64‑bit or smaller.  On systems without 128‑bit
   integer support the simple expression is used (it only overflows
   when the final result itself would overflow). */
static seq_t muldiv(seq_t x, seq_t y, seq_t d)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    __uint128_t prod = (__uint128_t)x * y;
    return (seq_t)(prod / d);
#else
    return (x * y) / d;
#endif
}

seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t factorial = 1;
    for (idx_t i = 1; i <= n; ++i)
        factorial *= (seq_t)i;

    /* Sum terms term_k = n! * n^k / k! */
    seq_t sum = 0;
    seq_t cur = factorial;        /* term_0 */

    for (idx_t k = 0; k < n; ++k) {
        sum += cur;
        if (k == n - 1) break;    /* no further term needed */

        /* next term = cur * n / (k+1) */
        cur = muldiv(cur, (seq_t)n, (seq_t)(k + 1));
    }

    return sum;
}

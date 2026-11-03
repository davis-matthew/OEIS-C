/*
 * A122247  Partial sums of A005187.
 * https://oeis.org/A122247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} Sum_{j=0..n} floor(k/2^j) Equivalent: a(n) =
 *            n*(n+1) - Σ_{k=1..n} popcount(k)
 * Range:     u8 n <= 16, u16 n <= 257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9915 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t popcount_sum(idx_t n)
{
    seq_t sum = 0;
    idx_t pow = 1;                           /* 2^i for current bit i            */
    while (pow <= n) {
        /* Number of integers in [1..n] having bit i set:                */
        /*   full = (n+1)/period  * 2^i  +  max(0, (n+1)%period - 2^i) */
        idx_t half   = pow;                   /* 2^i                              */
        idx_t period = half << 1;             /* 2^(i+1)                          */

        idx_t full = (n + 1) / period;        /* full periods                     */
        seq_t  add = (seq_t)full * half;      /* contribute from full periods     */

        idx_t rem = (n + 1) % period;         /* remainder after full periods     */
        if (rem > half)
            rem -= half;
        else
            rem = 0;
        add += (seq_t)rem;                    /* contribute from the incomplete part */

        sum += add;

        /* Prevent overflow of pow on next shift. */
        if (pow > n >> 1)
            break;
        pow <<= 1;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* n*(n+1) fits into seq_t (arbitrary precision if necessary). */
    seq_t total = (seq_t)n * ((seq_t)n + 1);
    seq_t popsum = popcount_sum(n);
    return total - popsum;
}

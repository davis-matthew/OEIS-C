/*
 * A008463  Take sum of squares of digits of previous term; start with 9.
 * https://oeis.org/A008463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of squares of the decimal digits of a(n‑1); a(1) = 9
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: The values of the sequence are all ≤ 145, so any seq_t
   (whether small integer or big‑int class) can safely be cast to
   uint64_t for digit extraction. */

#include <stdint.h>
#include <stddef.h>

/* compute sum of squares of decimal digits of a small integer value */
static inline seq_t digit_square_sum_generic(seq_t v)
{
    uint64_t w = (uint64_t)v;
    uint64_t sum = 0;
    while (w > 0)
    {
        uint64_t d = w % 10;
        sum += d * d;
        w /= 10;
    }
    return (seq_t)sum;
}

/* recurrence for A008463 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1 – return 0 for safety */
    if (n == 1)
        return (seq_t)9;
    if (n == 2)
        return (seq_t)81;
    if (n == 3)
        return (seq_t)65;
    if (n == 4)
        return (seq_t)61;

    /* compute the 8‑term cycle that begins with a(5) */
    seq_t cycle[8];
    seq_t prev = (seq_t)61;          /* a(4) */
    for (int i = 0; i < 8; ++i)
    {
        prev = digit_square_sum_generic(prev);
        cycle[i] = prev;
    }

    /* a(5) = cycle[0], a(6) = cycle[1], … , a(12) = cycle[7]   */
    /* for n ≥ 5, the index inside the cycle is (n‑5) mod 8 */
    return cycle[(size_t)(n - 5) & 7];   /* mask 7 works as %8 for positive n */
}

/*
 * A206093  Number of (n+1) X 8 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 258*a(n-3) for n>10. Initial values: a(1)=205416,
 *            a(2)=165936, a(3)=240192, a(4)=1645920, a(5)=8613504,
 *            a(6)=56633472, a(7)=408445632, a(8)=2199502080,
 *            a(9)=14566314240, a(10)=105251128704 This implementation uses
 *            only arithmetic on seq_t, so it works for both built‑in numeric
 *            types and ar...
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base values for indices 1..10 */
    static const seq_t base[10] = {
        (seq_t)205416ULL,     (seq_t)165936ULL,
        (seq_t)240192ULL,     (seq_t)1645920ULL,
        (seq_t)8613504ULL,    (seq_t)56633472ULL,
        (seq_t)408445632ULL,  (seq_t)2199502080ULL,
        (seq_t)14566314240ULL, (seq_t)105251128704ULL
    };

    if (n == 0) /* defined for safety, though not used in OEIS */
        return (seq_t)0;

    /* If n is within the base range, return pre‑computed value. */
    if (n <= 10)
        return base[n - 1];

    /* Find the largest index ≤ n that is ≤ 10
       by stepping back in multiples of 3. */
    idx_t m = n;
    while (m > 10)
        m -= 3U;

    /* Start from the base at index m, then multiply by 258
       for each jump of +3 until reaching n. */
    seq_t result = base[m - 1];
    for (idx_t k = m + 3; k <= n; k += 3U)
        result = result * (seq_t)258U;

    return result;
}

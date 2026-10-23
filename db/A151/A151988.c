/*
 * A151988  G.f.:
 *          (x*(x^4+1)*(x^2-x+1)*(x^2+x+1))/((x^4+x^3+x^2+x+1)*(x^4-x^3+x^2-x+1)*(x-1)^2).
 * https://oeis.org/A151988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth nonnegative integer that is NOT congruent to 5 or
 *            7 modulo 12. Equivalently, sequence of all integers except those
 *            with residue 5 or 7 modulo 12. The function computes a(n) by
 *            binary searching for the smallest m such that the count of
 *            allowed numbers <= m is at least n+1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Search bounds: density of allowed numbers is 5/6 ≈ 0.8333,
       so a(n) is roughly 1.2 * n.  Add a safety margin. */
    idx_t low  = 0;
    idx_t high = n * 12 / 10 + 20;          /* upper bound estimate */
    if (high < n) high = n * 2 + 20;        /* very small n safety */

    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;

        /* Count excluded numbers <= mid : those ≡5 or ≡7 (mod 12) */
        seq_t excluded = 0;
        if (mid >= 5) excluded += (seq_t)((mid - 5) / 12) + 1;
        if (mid >= 7) excluded += (seq_t)((mid - 7) / 12) + 1;

        /* Count allowed numbers <= mid */
        seq_t incl = (seq_t)mid + 1 - excluded;

        if (incl >= n + 1)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;   /* low is the nth allowed number */
}

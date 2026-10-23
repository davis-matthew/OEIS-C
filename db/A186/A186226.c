/*
 * A186226  Adjusted joint rank sequence of (g(j)) and (f(i)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          pentagonal numbers. Complement of A186225.
 * https://oeis.org/A186226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊(√(8·P_n+1)−1)/2⌋ + ⌊(1+√(1+24·P_n))/6⌋ − [P_n is
 *            triangular], where P_n = n(3n−1)/2 is the nth pentagonal number.
 *            This gives the rank of the nth pentagonal number in the merged
 *            list of pentagonal and triangular numbers, with pentagonal
 *            entries placed before triangular ones when equal
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute ⌊√x⌋ for an arbitrary precision unsigned integer type seq_t. */
static seq_t isqrt_seq(seq_t x)
{
    if (x <= 0)
        return (seq_t)0;
    seq_t low = 0;
    seq_t high = 1;
    /* Increase high until high·high > x, using overflow‑safe test. */
    while (high <= x / high) {
        high <<= 1;
    }
    /* Binary search for floor(sqrt(x)). */
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Pentagonal number P_n = n(3n−1)/2 */
    seq_t Pn = nn * ((seq_t)(3 * nn - 1)) / (seq_t)2;

    /* Count of triangular numbers ≤ Pn: ⌊(√(8·Pn+1)−1)/2⌋ */
    seq_t tmp1 = (seq_t)8 * Pn + (seq_t)1;
    seq_t sqrt1 = isqrt_seq(tmp1);
    seq_t t = (sqrt1 - (seq_t)1) / (seq_t)2;

    /* Count of pentagonal numbers ≤ Pn: ⌊(1+√(1+24·Pn))/6⌋ */
    seq_t tmp2 = (seq_t)24 * Pn + (seq_t)1;
    seq_t sqrt2 = isqrt_seq(tmp2);
    seq_t p = ((seq_t)1 + sqrt2) / (seq_t)6;

    /* Check if Pn is itself triangular. */
    seq_t k = (sqrt1 - (seq_t)1) / (seq_t)2;          /* candidate k */
    seq_t Tk = k * (k + (seq_t)1) / (seq_t)2;          /* T_k */
    seq_t tri_flag = (Tk == Pn) ? ((seq_t)1) : ((seq_t)0);

    return t + p - tri_flag;
}

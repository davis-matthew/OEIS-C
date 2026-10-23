/*
 * A030693  Smallest nontrivial extension of n-th cube which is a square.
 * https://oeis.org/A030693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest square > n³ whose decimal representation starts
 *            with the digits of n³ (i.e. the least square that is a
 *            non‑trivial extension of the n‑th cube)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Return ⌈√x⌉ for seq_t using floor_sqrt. */
static seq_t ceil_sqrt(seq_t x)
{
    seq_t s = floor_sqrt(x);
    seq_t sq = s * s;
    if (sq < x) {
        ++s;
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }

    /* n³ */
    seq_t cube = (seq_t)n;
    cube *= cube;
    cube *= (seq_t)n;

    /* try increasing numbers of appended decimal digits */
    seq_t pow10 = (seq_t)10;          /* 10^m with m = 1 initially */
    while (1) {
        seq_t lower = cube * pow10;
        seq_t upper = (cube + (seq_t)1) * pow10 - (seq_t)1;

        seq_t t_lo = ceil_sqrt(lower);
        seq_t t_hi = floor_sqrt(upper);

        if (t_lo <= t_hi) {
            return t_lo * t_lo;      /* smallest square in the interval */
        }
        pow10 *= (seq_t)10;          /* increase m */
    }
}

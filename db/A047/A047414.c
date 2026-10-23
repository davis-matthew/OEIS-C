/*
 * A047414  Numbers that are congruent to {0, 3, 4, 6} mod 8.
 * https://oeis.org/A047414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/4) + r[(n-1) mod 4] where r = {0, 3, 4,
 *            6}
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Index `n` is 1‑based.  All calls satisfy n ≥ 1. */
    static const seq_t offs[4] = {0, 3, 4, 6};
    idx_t m = n - 1;                    /* 0‑based position */
    seq_t base = ((seq_t)8 * ((seq_t)m / 4));
    seq_t val = base + offs[m % 4];
    return val;
}

/*
 * A047507  Numbers that are congruent to {0, 4, 6, 7} mod 8.
 * https://oeis.org/A047507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + rem where rem = 0,4,6,7 depending on
 *            (n-1)%4
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = n - 1;             /* zero‑based index */
    idx_t k = m / 4;             /* block number   */
    idx_t r = m % 4;             /* offset inside block */
    seq_t base = (seq_t)k * 8;   /* 8 * k is representable in seq_t */

    switch (r) {
        case 0: return base;           /* 8*k            */
        case 1: return base + 4;       /* 8*k + 4        */
        case 2: return base + 6;       /* 8*k + 6        */
        case 3: return base + 7;       /* 8*k + 7        */
    }
    return base; /* unreachable, keeps compiler quiet */
}

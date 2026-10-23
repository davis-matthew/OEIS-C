/*
 * A008856  Numbers n such that n^3 and n have same last 2 digits.
 * https://oeis.org/A008856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   For a given index n (1‑based), let m = n-1. Then a(n) is given
 *            by: a(n) = 100*(m/9) if m%9 == 0 a(n) = 100*(m/9) + 1 if m%9 ==
 *            1 a(n) = 100*(m/9) + 24 if m%9 == 2 a(n) = 100*(m/9) + 25 if m%9
 *            == 3 a(n) = 100*(m/9) + 49 if m%9 == 4 a(n) = 100*(m/9) + 51 if
 *            m%9 == 5 a(n) = 100*(m/9) + 75 if m%9 ==...
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset: a(1) corresponds to formula index m = 0 */
    idx_t m = n - 1;
    seq_t base = (seq_t)100 * (seq_t)(m / 9);
    switch (m % 9) {
        case 0: return base;
        case 1: return base + 1;
        case 2: return base + 24;
        case 3: return base + 25;
        case 4: return base + 49;
        case 5: return base + 51;
        case 6: return base + 75;
        case 7: return base + 76;
        case 8: return base + 99;
        default: return 0; /* unreachable */
    }
}

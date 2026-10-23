/*
 * A101426  Number of different cuboids with volume p^6 * q^n, where p,q are
 *          distinct prime numbers.
 * https://oeis.org/A101426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*(n+2)*(n+1) + [3|n?1:0]) / 3 + 2*floor(n/2) + 2
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute 7*(n+2)*(n+1) */
    seq_t base = 7 * (nn + 2) * (nn + 1);
    seq_t div_part;
    if ((nn % 3) == 0)
        div_part = (base + 1) / 3;
    else
        div_part = base / 3;
    /* 2*floor(n/2) */
    seq_t add_two_floor = (nn >> 1) * 2;
    return div_part + add_two_floor + 2;
}

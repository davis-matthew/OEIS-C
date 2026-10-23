/*
 * A008882  3x+1 sequence starting at 99.
 * https://oeis.org/A008882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3x+1 sequence starting at 99 */
/* a(0) = 99; for n > 0, a(n) = a(n-1)/2 if a(n-1) even, else 3*a(n-1)+1 */
seq_t a(idx_t n)
{
    seq_t value = 99;
    for (idx_t i = 1; i <= n; ++i) {
        if ((value & 1) == 0)          /* even */
            value /= 2;
        else
            value = value * 3 + 1;     /* odd */
    }
    return value;
}

/*
 * A084263  a(n) = (-1)^n/2+(n^2+n+1)/2.
 * https://oeis.org/A084263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + floor(n/2) + floor(n^2/2)
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently: a(n) = ((n^2 + n + 1) + (-1)^n)/2) */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t half = sn / 2;
    seq_t sq   = sn * sn;
    seq_t halfsq = sq / 2;
    return 1 + half + halfsq;
}

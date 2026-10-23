/*
 * A131380  a(3n) = 2n, a(3n+1) = 2n+2, a(3n+2) = 2n+1.
 * https://oeis.org/A131380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/3) + (n mod 3==0 ? 0 : n mod 3==1 ? 2 : 1)
 * Range:     u8 n <= 381, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _add[3] = {0, 2, 1};

seq_t a(idx_t n)
{
    idx_t q = n / 3;
    idx_t r = n % 3;
    return (seq_t)q * (seq_t)2 + _add[r];
}

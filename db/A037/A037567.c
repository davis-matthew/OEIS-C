/*
 * A037567  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,2,1.
 * https://oeis.org/A037567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(221/999 * 10^n) (integer part) Recurrence: a(1)=2;
 *            a(n)=10*a(n-1)+ (n^2 mod 3)+1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    static const int pat[3] = {2, 2, 1};

    for (idx_t i = 0; i < n; ++i) {
        val *= 10;
        val += pat[i % 3];
    }
    return val;
}

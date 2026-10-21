/*
 * A224234  Decimal expansion of number of feet in a meter.
 * https://oeis.org/A224234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(1)=3; for n≥2, a(n) = the (n‑2)-th digit after the decimal
 *            point of 1250/381
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t DEC_DIGITS[42] = {
    2,8,0,8,3,9,8,9,5,0,
    1,3,1,2,3,3,5,9,5,8,
    0,0,5,2,4,9,3,4,3,8,
    3,2,0,2,0,9,9,7,3,7,
    5,3
};

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    idx_t pos = (n - 2) % 42ULL;
    return DEC_DIGITS[pos];
}

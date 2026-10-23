/*
 * A129107  Decimal expansion of 42223444 = 84446888/2 (see A129106).
 * https://oeis.org/A129107
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (8 ≤ n ≤ 15) ? the (n-8)-th digit of 42223444 : 0
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t digits[8] = {
        (seq_t)4, (seq_t)2, (seq_t)2, (seq_t)2,
        (seq_t)3, (seq_t)4, (seq_t)4, (seq_t)4
    };
    if (n < (idx_t)8) return (seq_t)0;
    idx_t i = n - (idx_t)8;
    if (i < (idx_t)8) return digits[i];
    return (seq_t)0;
}

/*
 * A068028  Decimal expansion of 22/7.
 * https://oeis.org/A068028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, cons
 * Formula:   a(1)=3; for n>1, a(n)= {1,4,2,8,5,7}[(n-2) mod 6]
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    /* repeating decimal digits of 22/7 after the integer part */
    static const seq_t digits[6] = { (seq_t)1, (seq_t)4, (seq_t)2,
                                    (seq_t)8, (seq_t)5, (seq_t)7 };

    /* (n-2) is non‑negative for n>=2 */
    idx_t pos = (n - 2) % 6;
    return digits[pos];
}

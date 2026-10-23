/*
 * A021589  Decimal expansion of 1/585.
 * https://oeis.org/A021589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(0)=0, a(1)=0. For n≥2, a(n) is the digit of the decimal
 *            expansion of 1/585, which repeats with period 6 after the two
 *            leading zeros: a(n) = {1,7,0,9,4,0}[ (n-2) mod 6 ]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[6] = {1, 7, 0, 9, 4, 0};
    if (n < 2)
        return 0u;
    unsigned r = (unsigned)((n - 2) % 6);
    return pat[r];
}

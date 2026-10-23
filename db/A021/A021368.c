/*
 * A021368  Decimal expansion of 1/364.
 * https://oeis.org/A021368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021368: decimal digit of 1/364.
   a(0)=0, a(1)=0. For n≥2, the digits repeat the pattern
   {2,7,4,7,2,5} of length 6:
   a(n) = pattern[(n-2) % 6]. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    static const seq_t pattern[6] = {2, 7, 4, 7, 2, 5};
    idx_t k = (n - 2) % 6;
    return pattern[k];
}

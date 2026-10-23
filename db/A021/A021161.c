/*
 * A021161  Decimal expansion of 1/157.
 * https://oeis.org/A021161
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

/*  a(n) = the n-th digit (0‑based) in the decimal expansion of 1/157
    This computes the digit by repeated long division,
    without any dynamic allocation. */

seq_t a(idx_t n)
{
    /* the divisor is the constant 157 */
    const uint64_t divisor = 157ULL;

    /* remainder before extracting the current digit.
       Start with the leading 1 from the fraction 1/157. */
    uint64_t rem = 1ULL;

    /* advance n steps of the long division to reach the n-th digit */
    for (idx_t i = 0; i < n; ++i)
    {
        /* bring down a zero: multiply by 10 and reduce modulo divisor */
        rem = (rem * 10ULL) % divisor;
    }

    /* the current digit is floor(rem*10 / divisor) */
    seq_t digit = (seq_t)((rem * 10ULL) / divisor);
    return digit;
}

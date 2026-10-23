/*
 * A021382  Decimal expansion of 1/378.
 * https://oeis.org/A021382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit in the decimal expansion of 1/378 after the *
 *            decimal point. The digits repeat with period 6 * starting at
 *            a(1); a(0) is the leading zero. * Explicitly: a(0)=0, * a(n) =
 *            P[(n-1) % 6] for n>=1, * where P = {0,2,6,4,5,5}
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) {
        return 0;
    }
    /* Periodic digits of 1/378 after the first digit. */
    static const seq_t period[6] = {0, 2, 6, 4, 5, 5};
    return period[(n - 1u) % 6u];
}

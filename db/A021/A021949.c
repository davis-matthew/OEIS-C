/*
 * A021949  Decimal expansion of 1/945.
 * https://oeis.org/A021949
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

/* Sequence A021949: a(n) is the nth decimal digit of 1/945.
   The expansion is 0.001058201058201058..., so the first two
   digits are 0,0 and thereafter the digits repeat with period 6:
   1,0,5,8,2,0. */

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(0)=0, a(1)=0 */
        return 0;
    static const seq_t pat[6] = {1, 0, 5, 8, 2, 0};
    return pat[(n - 2) % 6];
}

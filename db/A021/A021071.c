/*
 * A021071  Decimal expansion of 1/67.
 * https://oeis.org/A021071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0) = 0; for n ≥ 1, a(n) = ⌊ (10^(n+1) / 67) ⌋ mod 10 which is
 *            obtained by performing n+1 iterations of long division
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Decimal expansion of 1/67.
    a(0) = integer part of 1/67 = 0.
    For n ≥ 1, a(n) equals the (n+1)-st digit in the decimal
    expansion of 1/67 (i.e. the digit produced after n+1 steps
    of the long‑division algorithm starting from remainder 1). */


seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* integer part */

    /* Current remainder in the long division of 1 by 67.
       The remainder is always < 67, so seq_t handles it safely. */
    seq_t r = 1;
    seq_t d = 0;

    /* Perform n+1 steps of the long‑division to reach the desired digit. */
    for (idx_t i = 0; i <= n; ++i)
    {
        r *= 10;                    /* bring down next 0 */
        d = r / 67;                 /* next decimal digit */
        r = r % 67;                 /* new remainder */
    }
    return d;                       /* digit at position n+1 after decimal point */
}

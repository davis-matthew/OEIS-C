/*
 * A021803  Decimal expansion of 1/799.
 * https://oeis.org/A021803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n-th integer in the expansion of 1/799, with a(0) =
 *            0 (integer part). For n ≥ 1 a(n) equals the (n+1)-st decimal
 *            digit of 1/799
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* integer part of 1/799 */

    uint64_t rem = 1;              /* current remainder */
    seq_t digit = 0;

    /* We need the (n+1)-st decimal digit, so perform n+1
       iterations of long‑division. */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                 /* bring down next decimal digit */
        digit = rem / 799;         /* current digit */
        rem %= 799;                /* update remainder */
    }
    return digit;
}

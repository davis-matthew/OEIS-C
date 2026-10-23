/*
 * A021476  Decimal expansion of 1/472.
 * https://oeis.org/A021476
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

/*  a(n) = the n-th digit after the decimal point of 1/472
 *  (offset 0: a(0)=0, a(1)=0, a(2)=2, a(3)=1, …) */
seq_t a(idx_t n)
{
    idx_t rem   = 1;    /* current remainder, 1 = numerator */
    seq_t digit = 0;    /* last produced digit */

    for (idx_t i = 0; i <= n; ++i) {
        rem   = rem * 10;                 /* bring down next digit */
        digit = (seq_t)(rem / 472);       /* next decimal digit */
        rem   = rem % 472;                /* update remainder */
    }
    return digit;
}

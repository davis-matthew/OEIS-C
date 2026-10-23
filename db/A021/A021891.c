/*
 * A021891  Decimal expansion of 1/887.
 * https://oeis.org/A021891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 887) mod 10 which is exactly the
 *            (n+1)-th decimal digit of 1/887 (digits after the decimal point,
 *            starting with a(0)=0)
 * Range:     u8 n <= 4, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the remainder after dividing some power of 10 by 887
       initially 1/887 gives remainder 1. */
    seq_t r = 1;
    seq_t d = 0;

    /* Perform long division n+1 times; after the last iteration
       'd' holds the desired digit. */
    for (idx_t i = 0; i <= n; ++i) {
        d = (r * 10) / 887;
        r = r * 10 - d * 887;
    }
    return d;
}

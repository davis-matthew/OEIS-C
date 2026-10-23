/*
 * A021689  Decimal expansion of 1/685.
 * https://oeis.org/A021689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n-th decimal digit of 1/685 (offset 0)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The remainder never exceeds 684, so seq_t is sufficient for all
       intermediate values. */
    seq_t r = (seq_t)1;   /* initial remainder is 1 (the numerator) */
    seq_t d = (seq_t)0;   /* the digit to be returned */

    /* Perform the usual long‑division steps n+1 times to reach the
       requested digit. */
    for (idx_t i = (idx_t)0; i <= n; ++i) {
        r *= (seq_t)10;
        d = r / (seq_t)685;
        r %= (seq_t)685;
    }

    return d;
}

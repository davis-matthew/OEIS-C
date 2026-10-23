/*
 * A037495  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,1.
 * https://oeis.org/A037495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor( (7*10^n) / 33 ) (digits are the first n terms of
 *            the periodic sequence 2,1,2,1,...)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 2, a(2) = 21, ... */
    if (n == 0) {
        return 0;
    }

    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* odd positions give digit '2', even positions give digit '1' */
        seq_t digit = (i & 1) ? 2 : 1;
        val = val * 10 + digit;
    }
    return val;
}

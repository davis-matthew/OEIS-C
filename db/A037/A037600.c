/*
 * A037600  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0.
 * https://oeis.org/A037600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1 * a(n) = 7 * a(n-1) + p[(n-1) mod 3], n > 1 * p = {1,
 *            3, 0} (the repeating base-7 digits)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;                            /* accumulator for a(n)       */
    static const seq_t p[3] = {1, 3, 0};      /* base‑7 digit pattern       */

    for (idx_t i = 0; i < n; ++i) {
        /* i corresponds to the (i+1)-th digit in the pattern   */
        val = val * 7 + p[i % 3];
    }
    return val;
}

/*
 * A037573  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,2.
 * https://oeis.org/A037573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) * 8 + d(n), a(0) = 0 * where d(n) = 1 if (n-1) mod
 *            3 == 1, otherwise d(n) = 2. * This reproduces the base‑8 digits
 *            2,1,2,2,1,2,2,1,2,… as described in OEIS
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 by the OEIS offset; if 0 is passed we return 0. */
    if (n == 0) return (seq_t)0;

    seq_t val = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit = ((i - 1) % 3 == 1) ? (seq_t)1 : (seq_t)2;
        val = val * (seq_t)8 + digit;
    }
    return val;
}

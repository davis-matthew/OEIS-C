/*
 * A037618  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3.
 * https://oeis.org/A037618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)*4 + d(n), with d(n) being the (n‑1)-mod‑3 element
 *            of the periodic sequence 2,0,3. a(0) = 0. This interprets the
 *            first n digits of that periodic sequence as a base‑4 integer
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;              /* start with 0 for n = 0 (unused) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t digit;
        switch ((i - 1) % 3)    /* sequence 2,0,3 repeats every 3 digits */
        {
        case 0:  digit = 2; break;
        case 1:  digit = 0; break;
        case 2:  digit = 3; break;
        }
        val = val * 4 + digit;   /* shift left by one base‑4 digit and add */
    }
    return val;
}

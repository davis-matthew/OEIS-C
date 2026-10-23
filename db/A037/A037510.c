/*
 * A037510  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0.
 * https://oeis.org/A037510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; for n>1, a(n) = a(n-1) * 9 + digit(n-1), where
 *            digit(k) = 1 if k%3==0, 2 if k%3==1, otherwise 0. This produces
 *            the base‑9 number with digits 1,2,0 repeating
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; 0 is a safe fallback */
    seq_t val = 1;                 /* a(1) = 1 */

    for (idx_t i = 1; i < n; ++i) {
        seq_t d;
        switch (i % 3) {
            case 0: d = 1; break;
            case 1: d = 2; break;
            default: d = 0; break;
        }
        val = val * 9 + d;
    }
    return val;
}

/*
 * A037778  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 3,1,0,2.
 * https://oeis.org/A037778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + digit, a(1)=3. The digits repeat with period
 *            3,1,0,2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t digit;
        switch (i % 4) {
            case 0: digit = 3; break;
            case 1: digit = 1; break;
            case 2: digit = 0; break;
            default: digit = 2; break;  /* i % 4 == 3 */
        }
        val = val * 10 + digit;
    }
    return val;
}

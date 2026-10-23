/*
 * A037576  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3.
 * https://oeis.org/A037576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(7*4^n/15). Equivalent recurrence: a(1)=1, a(2)=7,
 *            a(3)=29, a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3) for n >= 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           // undefined for n < 1
    if (n == 1) return 1;
    if (n == 2) return 7;
    if (n == 3) return 29;

    seq_t a_nm3 = 1; // a(n-3)
    seq_t a_nm2 = 7; // a(n-2)
    seq_t a_nm1 = 29; // a(n-1)

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a_n = 4 * a_nm1 + a_nm2 - 4 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

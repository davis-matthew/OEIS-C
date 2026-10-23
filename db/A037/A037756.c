/*
 * A037756  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1,0.
 * https://oeis.org/A037756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the base‑9 number consisting of the first n terms of the
 *            periodic digit sequence 2,3,1,0,2,3,1,0,…. Equivalently, a(1)=2
 *            and for n>1: a(n)=9*a(n‑1)+d(n), where d(n) = [2,3,1,0][(n‑1)%4]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const int period[4] = {2, 3, 1, 0};
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        int d = period[(i - 1) & 3];      /* (i‑1)%4 using bitwise AND */
        result = result * 9 + (seq_t)d;   /* multiply by base 9 and add digit */
    }
    return result;
}

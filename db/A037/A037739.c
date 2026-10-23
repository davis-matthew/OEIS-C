/*
 * A037739  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3,0.
 * https://oeis.org/A037739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6*a(n-1) + d_n, a(1)=2, d_n = (2,1,3,0) repeating
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) equals the number whose base‑6 expansion consists of the first n digits of the
   periodic sequence 2,1,3,0,2,1,3,0,… */
seq_t a(idx_t n)
{
    /* a(1)=2, for n>=1.  Behaviour for n==0 is irrelevant but handled */
    if (n == 0) {
        return (seq_t)0;
    }

    static const unsigned char period[4] = { 2, 1, 3, 0 };
    seq_t val = (seq_t)2;             /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* i-1 is zero based index into period */
        val = val * (seq_t)6 + period[(i - 1) & 3];
    }
    return val;
}

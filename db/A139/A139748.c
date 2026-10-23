/*
 * A139748  a(n) = Sum_{ k >= 0} binomial(n,5*k+3).
 * https://oeis.org/A139748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k>=0} binomial(n,5*k+3)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 5‑state recurrence derived from Pascal’s identity:
       T_r(n+1) = T_r(n) + T_{(r-1) mod 5}(n)   (r = 0..4)
       with initial values T_0(0)=1, T_r(0)=0 for r≠0.
       Set r = 3 to get a(n).  */
    seq_t t[5] = {0};
    t[0] = 1;                     /* C(0,0) */
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t nt0 = t[0] + t[4];
        seq_t nt1 = t[1] + t[0];
        seq_t nt2 = t[2] + t[1];
        seq_t nt3 = t[3] + t[2];
        seq_t nt4 = t[4] + t[3];
        t[0] = nt0;
        t[1] = nt1;
        t[2] = nt2;
        t[3] = nt3;
        t[4] = nt4;
    }
    return t[3];
}

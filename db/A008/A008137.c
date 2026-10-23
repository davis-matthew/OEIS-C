/*
 * A008137  Coordination sequence T1 for Zeolite Code LTA and RHO.
 * https://oeis.org/A008137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(5*m + k) = 40*m^2 + 16*k*m + c[k] where m = floor(n/5), k = n
 *            mod 5, and c[0]=1 if n=0 (i.e. m=0), otherwise c[0]=2; c[1]=4,
 *            c[2]=9, c[3]=17, c[4]=28
 * Range:     u8 n <= 12, u16 n <= 202, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute quotient and remainder */
    idx_t m = n / 5;
    idx_t k = n % 5;

    /* Two terms needed in the polynomial form */
    seq_t m_seq = (seq_t)m;
    seq_t k_seq = (seq_t)k;

    seq_t base = (seq_t)40 * m_seq * m_seq + (seq_t)16 * k_seq * m_seq;

    /* Constant offsets depending on k, with special case for k==0, m>0 */
    static const seq_t c[5] = { (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)17, (seq_t)28 };
    seq_t offset = (k == 0 && m > 0 ? (seq_t)2 : c[k]);

    return base + offset;
}

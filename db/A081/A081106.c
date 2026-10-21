/*
 * A081106  6th binomial transform of (1,1,0,0,0,0,...).
 * https://oeis.org/A081106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) – 36*a(n-2), with a(0)=1, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)7;
    }
    seq_t a_nm2 = (seq_t)1;    /* a(n‑2) for n=2 initially */
    seq_t a_nm1 = (seq_t)7;    /* a(n‑1) for n=2 initially */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = a_nm1 * 12 - a_nm2 * 36;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}

/*
 * A168210  a(n) = 6*n - a(n-1) + 1 with n>1, a(1)=0.
 * https://oeis.org/A168210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + 2 + 5*(-1)^n
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t res = nseq * ((seq_t)3) + ((seq_t)2);
    if (n & (idx_t)1) {
        res -= ((seq_t)5);
    } else {
        res += ((seq_t)5);
    }
    return res;
}

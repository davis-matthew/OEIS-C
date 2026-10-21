/*
 * A213483  Number of (w,x,y) with all terms in {0,...,n} and |w-x| + |x-y| >=
 *          w+x+y.
 * https://oeis.org/A213483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 33*n^2 + 71*n + 15 + (3*n+9)*((n+1) mod 2))/24
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t term = n3
               + (seq_t)33 * n2
               + (seq_t)71 * sn
               + (seq_t)15;
    seq_t parity = (sn + (seq_t)1) % (seq_t)2;
    term += ((seq_t)3 * sn + (seq_t)9) * parity;
    return term / (seq_t)24;
}

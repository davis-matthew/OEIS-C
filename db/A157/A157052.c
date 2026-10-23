/*
 * A157052  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 6.
 * https://oeis.org/A157052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^4 + 2*n^3 + 11*n^2 + 10*n + 12)/36
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 71, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn     = (seq_t)n;
    seq_t sn1    = sn + (seq_t)1;
    seq_t sn2    = sn * sn;
    seq_t sn3    = sn2 * sn;
    seq_t sn4    = sn3 * sn;
    seq_t poly   = sn4
                 + (seq_t)2 * sn3
                 + (seq_t)11 * sn2
                 + (seq_t)10 * sn
                 + (seq_t)12;
    seq_t num    = sn * sn1 * poly;
    return num / (seq_t)36;
}

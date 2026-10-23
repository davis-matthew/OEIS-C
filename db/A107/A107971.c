/*
 * A107971  a(n) = (n+1)(n+2)(n+3)(35n^3 + 153n^2 + 232n + 120)/720.
 * https://oeis.org/A107971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)(n+2)(n+3)(35*n^3 + 153*n^2 + 232*n + 120)/720
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t t1   = nn + 1;
    seq_t t2   = nn + 2;
    seq_t t3   = nn + 3;
    seq_t poly = 35*nn*nn*nn + 153*nn*nn + 232*nn + 120;
    seq_t res  = t1 * t2 * t3 * poly / 720;
    return res;
}

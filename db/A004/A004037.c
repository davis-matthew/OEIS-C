/*
 * A004037  The coding-theoretic function A(n,6,4).
 * https://oeis.org/A004037
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = J(n) - e(n), J(n) = floor(n/4*floor((n-1)/3)) (Schoenheim
 *            bound); e(n) = 1 if n == 7,10 (mod 12) with n != 10,19;
 *            e(9)=e(17)=1; e(8)=e(10)=e(11)=2; e(19)=3; e(n) = 0 otherwise
 *            (Brouwer's theorem on optimal K4-packings of K_n)
 * Range:     u8 n <= 56, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)((n - 1) / 3);
    seq_t j = (seq_t)n * m / 4;
    idx_t e = 0;
    if (n == 8 || n == 10 || n == 11) e = 2;
    else if (n == 19) e = 3;
    else if (n == 9 || n == 17) e = 1;
    else if (n % 12 == 7 || n % 12 == 10) e = 1;
    return j - (seq_t)e;
}

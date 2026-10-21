/*
 * A068181  a(n)=-1/b(2n) where 1/(e^y-e^(y/3))= sum(i=-1,inf,b(i)*y^i).
 * https://oeis.org/A068181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9^n * (2n)! (equivalently, a(0)=1 and for n≥1,
 *            a(n)=a(n‑1)·9·(2n)·(2n‑1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        res *= (seq_t)9;
        res *= (seq_t)(2 * i);
        res *= (seq_t)(2 * i - 1);
    }
    return res;
}

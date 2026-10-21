/*
 * A063238  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          87 ).
 * https://oeis.org/A063238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 for n=1, 14 for n=2, 24 for n=3; for n>=4: a(n) = 32 +
 *            28*floor((n-4)/3) + 10 if (n-4)%3 == 1, 20 if (n-4)%3 == 2, 0
 *            otherwise
 * Range:     u8 n <= 27, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)5;
    if (n == (idx_t)2) return (seq_t)14;
    if (n == (idx_t)3) return (seq_t)24;

    idx_t m = n - (idx_t)4;
    idx_t blocks = m / (idx_t)3;
    idx_t rem = m % (idx_t)3;

    seq_t res = (seq_t)32 + (seq_t)blocks * (seq_t)28;
    if (rem == (idx_t)1) res += (seq_t)10;
    else if (rem == (idx_t)2) res += (seq_t)20;

    return res;
}

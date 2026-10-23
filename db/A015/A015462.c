/*
 * A015462  q-Fibonacci numbers for q=5, scaling a(n-2).
 * https://oeis.org/A015462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=1; a(n) = a(n-1) + 5^(n-2)*a(n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t pow5 = (seq_t)1;      /* 5^(i-2) for current i */
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = a1;
        an += pow5 * a0;
        a0 = a1;
        a1 = an;
        pow5 *= (seq_t)5;
    }
    return an;
}

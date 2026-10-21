/*
 * A097839  Chebyshev polynomials S(n,83).
 * https://oeis.org/A097839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 83*a(n-1) - a(n-2); a(0)=1, a(1)=83
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)83;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)83;
    idx_t i;
    for (i = 2; i <= n; ++i) {
        seq_t next = (seq_t)83 * a1 - a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}

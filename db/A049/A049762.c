/*
 * A049762  a(n) = Sum_{k=1..n} T(n,k), array T as in A049761.
 * https://oeis.org/A049762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (n^3 mod i)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t cube = n_seq * n_seq * n_seq;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sum += cube % (seq_t)i;
    }
    return sum;
}

/*
 * A126972  Number of distinct values taken by the entropy for permutations of
 *          [1..n], where the entropy of a permutation pi is Sum_{k=1..n}
 *          (pi(k)-k)^2.
 * https://oeis.org/A126972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n≥4, a(n) = 1 + binomial(n+1,3); a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=4
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) {
        static const seq_t table[] = {1, 1, 2, 4};
        return table[n];
    }
    seq_t N = (seq_t)n;
    seq_t num = (N - 1) * N * (N + 1);
    num /= 6;
    return num + 1;
}

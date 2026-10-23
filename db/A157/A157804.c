/*
 * A157804  a(n) = 1482401250*n^2 - 2793393900*n + 1315947601.
 * https://oeis.org/A157804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1482401250*n^2 - 2793393900*n + 1315947601
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long s = (long long)n;
    long long res = 1482401250LL * s * s - 2793393900LL * s + 1315947601LL;
    return (seq_t)res;
}

/*
 * A220747  a(n) = (2*n+1)!! / ((floor((n-1)/3)*2+1))!!
 * https://oeis.org/A220747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)!! / ((floor((n-1)/3)*2+1))!!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long m;
    if (n == 0)
        m = -1;
    else
        m = (long long)(n - 1) / 3;

    idx_t start = (idx_t)(m + 1);
    seq_t result = 1;
    seq_t odd = (seq_t)start * 2 + 1;

    for (idx_t i = start; i <= n; ++i) {
        result *= odd;
        odd += 2;
    }
    return result;
}

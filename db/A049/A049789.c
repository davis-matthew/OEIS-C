/*
 * A049789  a(n) = T(n,n-4), array T as in A049783.
 * https://oeis.org/A049789
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n-8} mod(n-4, floor((n-8)/j))
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n ≤ 8 the sum is empty (no terms). */
    if (n <= 8)
        return 0;

    idx_t n8 = n - 8;          /* n-8, safe since n ≥ 9 here */
    idx_t n_minus_4 = n - 4;   /* n-4 */

    seq_t sum = 0;
    for (idx_t j = 1; j <= n8; ++j) {
        idx_t div = n8 / j;                 /* floor((n-8)/j), ≥ 1 */
        seq_t rem = (seq_t)n_minus_4 % (seq_t)div;
        sum += rem;
    }
    return sum;
}

/*
 * A008499  Number of 5-dimensional centered tetrahedral numbers.
 * https://oeis.org/A008499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+6,6) - C(n,6) (C(n,k)=0 for n<k)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 153, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom6(idx_t n)
{
    if (n < 6) return 0;
    __int128 res = 1;
    for (idx_t i = 0; i < 6; ++i) {
        res = res * (n - i);
        res = res / (i + 1);
    }
    return (seq_t)res;        /* cast preserves value for all seq_t types   */
}

seq_t a(idx_t n)
{
    return binom6(n + 6) - binom6(n);
}

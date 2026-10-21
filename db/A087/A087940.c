/*
 * A087940  a(n) = Sum_{k=0..n} binomial(n+(-1)^k, k).
 * https://oeis.org/A087940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^(n-2) - (n%2) for n>1, a(1)=1 (OEIS A087940)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* Compute 5 * 2^(n-2). The loop runs (n-2) times. */
    seq_t val = 5;
    for (idx_t i = 0; i < n - 2; ++i)
        val *= 2;

    /* Subtract 1 when n is odd (since (1-(-1)^n)/2 = 1 for odd n). */
    if (n & 1)
        val -= 1;

    return val;
}

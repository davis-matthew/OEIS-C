/*
 * A016211  Expansion of 1/((1-x)(1-3x)(1-6x)).
 * https://oeis.org/A016211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 18*a(n-2) + 1 * Initial values: a(0) = 1, a(1)
 *            = 10
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)10;  /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)9;
        cur -= prev2 * (seq_t)18;
        cur += (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}

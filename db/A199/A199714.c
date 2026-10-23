/*
 * A199714  A puzzle by Ron Hoeflin.
 * https://oeis.org/A199714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(1) + Σ_{k=1}^{n-1} v[(k-1) mod 9] , where v =
 *            [9,14,7,5,14,9,15,21,19] (alphabet positions of “ingenious”),
 *            a(1) = 0. Equivalent: a(n) = ⌊(n-1)/9⌋·113 + Σ_{i=0}^{(n-1)%9-1}
 *            v[i]
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Alphabet positions of "ingenious" */
    static const seq_t v[9] = {9, 14, 7, 5, 14, 9, 15, 21, 19};

    /* n >= 1 by problem statement */
    idx_t m      = n - 1;                     /* number of differences to sum */
    seq_t cycles = (seq_t)(m / 9);            /* full cycles of 9 differences */
    idx_t rem    = (idx_t)(m % 9);            /* remaining differences */

    /* Sum of complete cycles */
    seq_t sum = cycles * (seq_t)113;          /* 113 = sum of one full cycle */

    /* Sum of the first 'rem' differences */
    seq_t add = 0;
    for (idx_t i = 0; i < rem; ++i)
        add += v[i];

    return sum + add;
}

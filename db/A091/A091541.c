/*
 * A091541  Four times triple factorials (3*n-2)!!! with leading 1 added.
 * https://oeis.org/A091541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * Π_{i=1}^{n-1} (3*i-2), with a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                    /* a(0) */
    seq_t prod = (seq_t)1;                  /* product of terms */
    for (idx_t i = 1; i < n; ++i) {         /* i runs 1..n‑1 */
        seq_t term = (seq_t)3 * (seq_t)i - (seq_t)2;
        prod *= term;
    }
    prod *= (seq_t)4;                       /* multiply by 4 at the end */
    return prod;
}

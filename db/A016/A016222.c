/*
 * A016222  Expansion of 1/((1-x)(1-4x)(1-6x)).
 * https://oeis.org/A016222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=11, a(n)=10*a(n-1)-24*a(n-2)+1 for n≥2 */
seq_t
a(idx_t n)
{
    /* base cases */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)11;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)11;  /* a(1) */
    seq_t cur;

    /* iterate from i = 2 up to n */
    for (idx_t i = 2u; i <= n; ++i) {
        cur = (seq_t)10 * prev1 - (seq_t)24 * prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

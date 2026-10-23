/*
 * A022823  a(n) = [ (2n+1)/(n-1) ] + [ (2n+2)/(n-2) ] + ... + [ (3n-1)/1 ].
 * https://oeis.org/A022823
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [ (2n+1)/(n-1) ] + [ (2n+2)/(n-2) ] + ... + [ (3n-1)/1 ]
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* behaviour below the offset is irrelevant */

    seq_t sum = 0;                  /* result accumulator */

    for (idx_t i = 1; i < n; ++i) {
        seq_t num = (seq_t)n * 2 + (seq_t)i;   /* 2n + i          */
        seq_t den = (seq_t)n - (seq_t)i;       /* n - i           */
        sum += num / den;                       /* floor division  */
    }

    return sum;
}

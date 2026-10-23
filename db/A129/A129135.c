/*
 * A129135  Number of permutations of [n] with exactly 5 fixed points.
 * https://oeis.org/A129135
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) - (-1)^n*binomial(n,5), a(n)=0 for 0 <= n < 5
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 15, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)                       /* below offset, result is 0 */
        return 0;

    seq_t prev = 0;                  /* a(4) = 0 */
    seq_t bin  = 1;                  /* binomial(5,5) */

    for (idx_t i = 5; i <= n; ++i) {
        if (i > 5)
            bin = ((seq_t)i * bin) / (seq_t)(i - 5);   /* bin = C(i,5) */

        if ((i & 1) == 0)            /* even n: subtract */
            prev = (seq_t)i * prev - bin;
        else                         /* odd n: add */
            prev = (seq_t)i * prev + bin;
    }
    return prev;
}

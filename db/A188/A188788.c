/*
 * A188788  Number of 5-step self-avoiding walks on an n X n X n X n 4-cube
 *          summed over all starting positions.
 * https://oeis.org/A188788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2696*n^4 - 9968*n^3 + 11424*n^2 - 4416*n + 384 for n>3
 *            with initial terms: a(1)=0, a(2)=1536, a(3)=39408
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 36, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1536;
    if (n == 3) return (seq_t)39408;

    /* for n>=4 use the closed‑form polynomial */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;      /* n^2 */
    seq_t n3 = n2 * sn;      /* n^3 */
    seq_t n4 = n3 * sn;      /* n^4 */

    seq_t res = (seq_t)2696 * n4;   /* 2696*n^4 */
    res -= (seq_t)9968 * n3;        /* -9968*n^3 */
    res += (seq_t)11424 * n2;       /* +11424*n^2 */
    res -= (seq_t)4416 * sn;        /* -4416*n   */
    res += (seq_t)384;             /* +384      */

    return res;
}

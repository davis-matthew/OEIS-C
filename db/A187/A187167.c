/*
 * A187167  Number of 6-step self-avoiding walks on an n X n X n cube summed
 *          over all starting positions.
 * https://oeis.org/A187167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 3534*n^3 - 15366*n^2 + 19536*n - 7056 (n > 4) * Initial
 *            values: a(1)=0, a(2)=240, a(3)=9504, a(4)=51504
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset and small indices explicitly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)240;
    if (n == 3) return (seq_t)9504;
    if (n == 4) return (seq_t)51504;
    if (n == 5) return (seq_t)148224;  /* explicit to avoid floating-point */
    if (n == 6) return (seq_t)320328;
    if (n == 7) return (seq_t)588924;
    if (n == 8) return (seq_t)975216;

    /* for n >= 5 the cubic polynomial holds; use only integer ops */
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;          /* n^2 */
    seq_t n3  = n2 * sn;          /* n^3 */

    return  (seq_t)3534 * n3
          - (seq_t)15366 * n2
          + (seq_t)19536 * sn
          - (seq_t)7056;
}

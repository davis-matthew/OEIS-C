/*
 * A028053  Expansion of 1/((1-3x)(1-5x)(1-6x)(1-7x)).
 * https://oeis.org/A028053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=21, a(2)=280, a(3)=3030,
   a(n) = 21*a(n-1) - 161*a(n-2) + 531*a(n-3) - 630*a(n-4)   (n ≥ 4) */
seq_t a(idx_t n)
{
    /* handle the first four values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)280;
    if (n == 3) return (seq_t)3030;

    /* sliding window of the last four terms */
    seq_t v0 = (seq_t)1,   /* a(n-4)   */
          v1 = (seq_t)21,  /* a(n-3)   */
          v2 = (seq_t)280, /* a(n-2)   */
          v3 = (seq_t)3030,/* a(n-1)   */
          vn;             /* a(n)     */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(n) = 21*a(n-1) - 161*a(n-2) + 531*a(n-3) - 630*a(n-4)   */
        vn = v3 * (seq_t)21 + v1 * (seq_t)531 - v2 * (seq_t)161 - v0 * (seq_t)630;

        /* shift the window for the next iteration */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = vn;
    }

    return vn;
}

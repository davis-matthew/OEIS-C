/*
 * A054878  Number of closed walks of length n along the edges of a
 *          tetrahedron based at a vertex.
 * https://oeis.org/A054878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = (3^n + (-1)^n*3)/4 Recurrence: a(0)=1, a(1)=0; for n≥2
 *            a(n)=2*a(n-1)+3*a(n-2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* a(n-2) and a(n-1) are kept in separate variables. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)0;   /* a(1) */
    seq_t cur;

    /* iterate up to the required index */
    for (idx_t i = 2; i <= n; ++i) {
        /* cur = 2*prev1 + 3*prev2, computed with only addition
           to avoid any intermediate overflow on the 64‑bit build. */
        cur = prev1 + prev1;                         /* 2*prev1 */
        cur = cur + (prev2 + prev2 + prev2);         /* +3*prev2 */

        /* shift window for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

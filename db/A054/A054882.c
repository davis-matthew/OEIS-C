/*
 * A054882  Closed walks of length n along the edges of a dodecahedron based
 *          at a vertex.
 * https://oeis.org/A054882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = (5 + 3^n + (-1)^n*2^(n+2) + 3*(1+(-1)^n)*sqrt(5)^n +
 *            4*0^n)/20. Recurrence (valid for n≥6): a(n) = 2*a(n-1) +
 *            10*a(n-2) - 16*a(n-3) - 25*a(n-4) + 30*a(n-5). Initial values:
 *            a(0)=1, a(1)=0, a(2)=3, a(3)=0, a(4)=15, a(5)=6
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Remember: all arithmetic is performed in type seq_t. The function never
 * assumes any particular size for seq_t, and it does not use dynamic
 * allocation, so it compiles cleanly for both plain integers and
 * arbitrary‑precision integer classes. */

seq_t a(idx_t n)
{
    /* Base cases for n ≤ 5 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)15;
    if (n == 5) return (seq_t)6;

    /* Circular buffer holding the last five computed values:
     * v[0] = a(n-5), v[1] = a(n-4), … , v[4] = a(n-1)   (before step)
     * After computing a(n) the buffer is shifted.
     */
    seq_t v[5];
    v[0] = (seq_t)0; /* placeholder for a(1) when starting at n=6 */
    v[1] = (seq_t)3;
    v[2] = (seq_t)0;
    v[3] = (seq_t)15;
    v[4] = (seq_t)6;

    for (idx_t i = 6; i <= n; ++i)
    {
        /* a(i) = 2*v[4] + 10*v[3] - 16*v[2] - 25*v[1] + 30*v[0] */
        seq_t next = ((seq_t)2 * v[4] + (seq_t)10 * v[3]
                   - (seq_t)16 * v[2] - (seq_t)25 * v[1]
                   + (seq_t)30 * v[0]);

        /* shift the buffer to make room for next */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }

    return v[4];
}

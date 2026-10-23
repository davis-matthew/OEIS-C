/*
 * A135622  16*Area^2 of integer triangles [A070080(n),A070081(n),A070082(n)].
 * https://oeis.org/A135622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (u+v+w)*(-u+v+w)*(u-v+w)*(u+v-w) where u = A070080(n), v
 *            = A070081(n), w = A070082(n)
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerate integer triangles in order of increasing perimeter, then
   increasing sides (u <= v <= w).  Return the value of the formula
   for the n-th triangle. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t count = 0;          /* number of triangles seen so far */

    for (idx_t p = 3; ; ++p)  /* perimeter */
    {
        for (idx_t u = 1; u <= p / 3; ++u)   /* shortest side */
        {
            idx_t maxV = (p - u) / 2;        /* enforce v <= w */
            for (idx_t v = u; v <= maxV; ++v)
            {
                idx_t w = p - u - v;
                if (u + v <= w) continue;   /* not a triangle */

                ++count;                    /* valid triangle */
                if (count == n)
                {
                    /* compute the formula using seq_t arithmetic */
                    seq_t su = (seq_t)u;
                    seq_t sv = (seq_t)v;
                    seq_t sw = (seq_t)w;

                    seq_t s1 = su + sv + sw;
                    seq_t s2 = sv + sw - su;
                    seq_t s3 = su + sw - sv;
                    seq_t s4 = su + sv - sw;

                    return (s1 * s2 * s3 * s4);
                }
            }
        }
    }
}

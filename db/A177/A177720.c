/*
 * A177720  Number of line segments connecting exactly 4 points in an n x n
 *          grid of points.
 * https://oeis.org/A177720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * Σ_{dx,dy∈Z} (n-|dx|)*(n-|dy|) with gcd(|dx|,|dy|)=3
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of line segments connecting exactly 4 points in an n×n grid. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0u) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 3u)
        return 0;  /* early exit for the first three terms */

    seq_t N = (seq_t)n;          /* use seq_t for arithmetic */
    idx_t   M = (n - 1u) / 3u;   /* max a,b with 3*a < n and 3*b < n */

    seq_t sum = 0;

    for (idx_t a = 0u; a <= M; ++a) {
        seq_t valA = N - (seq_t)(3u * a);
        for (idx_t b = 0u; b <= M; ++b) {
            if (a == 0u && b == 0u)
                continue;                              /* skip zero vector */
            if (gcd_idx(a, b) != 1u)
                continue;                              /* must have gcd(a,b)=1 */

            seq_t valB = N - (seq_t)(3u * b);
            seq_t term = valA * valB;

            /* multiplicity of sign choices */
            seq_t w = (a == 0u || b == 0u) ? 2u : 4u;

            sum += w * term;
        }
    }

    return sum / 2u;  /* unordered segments */
}

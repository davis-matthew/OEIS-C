/*
 * A066955  Number of unordered solutions of x*y + y*z + z*x = n, x,y,z > 0.
 * https://oeis.org/A066955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{(x,y,z) : x,y,z>0, x≤y≤z, xy+yz+zx = n}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t x = 1; x <= n / x; ++x)          /* x≤√n */
    {
        for (idx_t y = x; y <= n / x; ++y)      /* y≥x, xy≤n */
        {
            idx_t xy  = x * y;                  /* xy≤n */
            idx_t num = n - xy;
            if (num <= 0)  /* z would be ≤0, no further y gives positive num */
                break;
            idx_t denom = x + y;
            if (num % denom != 0)
                continue;
            idx_t z = num / denom;
            if (z >= y)          /* keep triples non‑decreasing */
                ++cnt;
        }
    }
    return cnt;
}

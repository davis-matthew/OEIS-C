/*
 * A212060  Number of (w,x,y,z) with all terms in {1,...,n} and w=x*y*z-2.
 * https://oeis.org/A212060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #((w,x,y,z)∈[1..n]^4 | w = x*y*z - 2)
 * Range:     u8 n <= 27, u16 n <= 1897, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t limit = (seq_t)n + 2;   /* w = product - 2  =>  product ≤ n+2 */
    seq_t total = 0;

    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; y <= n; ++y) {
            seq_t prod = (seq_t)x * (seq_t)y;
            if (prod > limit)           /* product increases with y */
                break;
            seq_t zmax = limit / prod;
            if (zmax > (seq_t)n)
                zmax = n;
            total += zmax;
        }
    }

    total -= (seq_t)4;   /* remove the 4 triples with product ≤ 2 */
    return total;
}

/*
 * A212059  Number of (w,x,y,z) with all terms in {1,...,n} and w=x*y*z-1.
 * https://oeis.org/A212059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # {(x,y,z)∈[1,n]^3 : x·y·z ≤ n+1} – 1 (subtract the
 *            single invalid triple (1,1,1) when n≥1)
 * Range:     u8 n <= 28, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0)          /* a(0) = 0 */
        return 0;

    idx_t bound = n + 1;                 /* n+1 fits in idx_t */
    seq_t bound_seq = (seq_t)bound;

    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; y <= n; ++y) {
            idx_t prod = x * y;          /* safe: idx_t holds up to 64 bit */
            if (prod > bound)            /* product exceeds n+1 → stop y-loop */
                break;
            seq_t z_max = bound_seq / (seq_t)prod;
            if (z_max > (seq_t)n)
                z_max = (seq_t)n;        /* z cannot exceed n */
            count += z_max;
        }
    }

    count -= 1;                          /* subtract (1,1,1) */
    return count;
}

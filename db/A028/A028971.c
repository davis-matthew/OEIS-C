/*
 * A028971  Theta series of quadratic form with Gram matrix [ 3, 1, 1; 1, 3,
 *          1; 1, 1, 3 ].
 * https://oeis.org/A028971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y,z)∈ℤ³ | 3(x²+y²+z²)+2(xy+xz+yz)=n }
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root for unsigned 64‑bit integers (floor) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0, bit = 1ULL << 62;          /* largest power of 4 <= n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Count representations of n by the quadratic form */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* bound on |x|, |y|, |z| : each ≤ sqrt(n/2) + 1 */
    idx_t bound = isqrt_u64(n / 2) + 1;

    /* iterate over all signed triples in the cube [-bound, bound]^3 */
    for (int64_t x = -(int64_t)bound; x <= (int64_t)bound; ++x) {
        for (int64_t y = -(int64_t)bound; y <= (int64_t)bound; ++y) {
            for (int64_t z = -(int64_t)bound; z <= (int64_t)bound; ++z) {
                __int128 q =
                    (__int128)3 * x * x +
                    (__int128)3 * y * y +
                    (__int128)3 * z * z +
                    (__int128)2 * x * y +
                    (__int128)2 * x * z +
                    (__int128)2 * y * z;
                if ((idx_t)q == n)
                    ++count;
            }
        }
    }

    return count;
}

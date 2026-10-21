/*
 * A071694  Number of ways to write n as n = x*y*z/(x+y+z) 1 <= x <= y <= z <=
 *          n.
 * https://oeis.org/A071694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = #{ (x,y,z) | 1 ≤ x ≤ y ≤ z ≤ n,  n = x*y*z/(x+y+z) }
 */
seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;                 /* offset is 1, value at 0 is unused */

    uint64_t N = (uint64_t)n;

    for (uint64_t x = 1; x <= N; ++x) {
        for (uint64_t y = x; y <= N; ++y) {

            uint64_t xy = x * y;
            int64_t denom_int = (int64_t)xy - (int64_t)N;
            if (denom_int <= 0) continue;            /* xy must be > n for a positive z */

            uint64_t denom = (uint64_t)denom_int;

            /* z = n*(x+y)/(xy-n) must be integer */
            __int128 num = (__int128)N * (x + y);     /* numerator fits in 64 bit, but use 128 for safety */
            if (num % denom != 0) continue;
            uint64_t z = (uint64_t)(num / denom);

            if (z < y || z > N) continue;            /* maintain ordering and bounds */
            ++count;
        }
    }

    return count;
}

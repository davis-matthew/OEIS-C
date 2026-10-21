/*
 * A080037  a(0)=2; for n > 0, a(n) = n + floor(sqrt(4n-3)) + 2.
 * https://oeis.org/A080037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n + floor(sqrt(4n-3)) + 2 (a(0) = 2)
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)2;
    }

    /* 4n-3 in 128‑bit to avoid overflow */
    unsigned __int128 val = (unsigned __int128)4 * n - (unsigned __int128)3;

    /* binary search for floor(sqrt(val)) */
    uint64_t lo = 0;
    uint64_t hi = 1;
    while ((unsigned __int128)hi * hi <= val) {
        hi <<= 1;
    }
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= val) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    uint64_t root = lo;

    /* compute n + root + 2 safely in 128 bits, then cast to seq_t */
    unsigned __int128 sum = (unsigned __int128)n + (unsigned __int128)root + (unsigned __int128)2;
    return (seq_t)sum;
}

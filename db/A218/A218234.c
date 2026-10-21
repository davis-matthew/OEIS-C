/*
 * A218234  Infinitesimal generator for padded Pascal matrix A097805 (as lower
 *          triangular matrices).
 * https://oeis.org/A218234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   a(n) = (sqrt(8*n+17) - 5)/2 if 8*n+17 is a perfect square,
 *            otherwise a(n) = 0
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for unsigned __int128 using binary search. */
static unsigned __int128 isqrt_uint128(unsigned __int128 x) {
    unsigned __int128 lo = 0;
    unsigned __int128 hi = ((unsigned __int128)1 << 64) - 1;  /* sqrt(2^128-1) < 2^64 */

    while (lo <= hi) {
        unsigned __int128 mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq  = mid * mid;

        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else {
            if (mid == 0) break;  /* avoid underflow */
            hi = mid - 1;
        }
    }
    return hi;  /* floor of the square root */
}

seq_t a(idx_t n) {
    /* Compute m = 8*n + 17 in 128‑bit arithmetic. */
    unsigned __int128 m = (unsigned __int128)8 * n + 17;

    /* Integer square root of m. */
    unsigned __int128 s = isqrt_uint128(m);

    /* Check if m is a perfect square. */
    if (s * s != m) {
        return (seq_t)0;
    }

    /* s will be at least 5 here (for n=1); no underflow occurs. */
    unsigned __int128 val = (s - 5) / 2;
    return (seq_t)val;
}

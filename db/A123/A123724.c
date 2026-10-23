/*
 * A123724  Partial sums of (-1)^floor(n*2^(1/3)).
 * https://oeis.org/A123724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, sign, look
 * Formula:   a(n) = sum_{k=1}^{n} (-1)^floor(k*∛2)
 * Range:     i8 n <= 9967, i16 n <= 9967, i32 n <= 9967, i64 n <= 9967,
 *            i128 n <= 9967, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor cube root of an __int128 integer */
static seq_t cube_root_floor(__int128 x) {
    __int128 lo = 0, hi = 1;
    /* Find an upper bound */
    while (hi * hi * hi <= x) hi <<= 1;
    /* Binary search for largest m with m^3 <= x */
    while (lo + 1 < hi) {
        __int128 mid = (lo + hi) >> 1;
        if (mid * mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

seq_t a(idx_t n) {
    /* If called with n == 0, behavior is irrelevant; return 0 */
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* t = 2 * k^3  (fits in 128 bits for the required n-range) */
        __int128 kk = k;            /* promote to 128-bit for safety */
        __int128 t  = 2 * kk * kk * kk;
        seq_t f = cube_root_floor(t);  /* f = floor(k*∛2) */
        if (f & 1) sum -= 1;                 /* odd -> -1 */
        else       sum += 1;                 /* even -> +1 */
    }
    return sum;
}

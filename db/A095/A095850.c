/*
 * A095850  Triangle read by rows: T(n,k) = k^((n-k+1)^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = k ^ ((row - k + 1)^2) where * row = ceil( ( sqrt(8*n + 1)
 *            - 1 ) / 2 ) * k = n - (row-1)*row / 2 (1 ≤ k ≤ row)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* integer square‑root (floor) for unsigned 64‑bit values */
static inline idx_t isqrt_uint64(idx_t v) {
    if (v < 2) return v;
    idx_t res = 0, bit = 1ULL << 62;          /* highest power of 4 ≤ v   */
    while (bit > v) bit >>= 2;
    while (bit) {
        if (v >= res + bit) {
            v -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Offset = 1, so n ≥ 1 in normal use.  For n == 0 we return 0. */
    if (n == 0) return (seq_t)0;

    /* Find the row number r such that (r-1)*r/2 < n ≤ r*(r+1)/2. */
    idx_t root = isqrt_uint64(1 + 8 * n);          /* floor(sqrt(1+8n))   */
    idx_t r = (root - 1) / 2;                      /* floor((sqrt-1)/2)  */
    if ( ((unsigned __int128)r * (r + 1)) / 2 < n ) ++r;

    /* Position of n inside the row.  k ranges from 1 to r. */
    idx_t prev = ((unsigned __int128)(r - 1) * r) / 2;
    idx_t k = n - prev;                            /* 1 ≤ k ≤ r           */

    /* Exponent e = (r - k + 1)^2.  Compute as idx_t. */
    idx_t t = r - k + 1;
    idx_t e = t * t;

    /* Power k^e with exponentiation by squaring. */
    seq_t base   = (seq_t)k;
    seq_t result = (seq_t)1;
    while (e > 0) {
        if (e & 1) result = result * base;
        if (e > 1) base = base * base;
        e >>= 1;
    }
    return result;
}

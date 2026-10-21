/*
 * A094761  a(n) = n + (square excess of n).
 * https://oeis.org/A094761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n - floor(sqrt(n))^2) = 2*n - floor(sqrt(n))^2
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_sqrt_u64(idx_t n)
{
    /* integer square root for unsigned 64‑bit numbers, works on all platforms */
    unsigned long long r = 0, bit = 1ULL << 62;      /* highest power of 4 <= n */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (idx_t)r;
}

seq_t a(idx_t n)
{
    idx_t k = floor_sqrt_u64(n);          /* k = floor(sqrt(n)) */
    seq_t k2 = (seq_t)k * (seq_t)k;      /* k^2 in seq_t arithmetic */
    seq_t nn = (seq_t)n;                  /* cast n to seq_t */
    return nn + (nn - k2);                /* 2*n - k^2 */
}

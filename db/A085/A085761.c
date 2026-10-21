/*
 * A085761  Number of triangular numbers between n and 2n (inclusive).
 * https://oeis.org/A085761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of triangular numbers T_k = k(k+1)/2 with n ≤ T_k
 *            ≤ 2n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;             /* largest power of 4 ≤ x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t disc1 = 1 + 8 * n;
    idx_t disc2 = 1 + 16 * n;

    idx_t s1 = isqrt(disc1);
    idx_t s2 = isqrt(disc2);

    idx_t k_start = (s1 - 1) / 2;
    /* ceil adjustment for k_start */
    if ((s1 - 1) % 2 != 0 || s1 * s1 < disc1)
        ++k_start;

    idx_t k_end = (s2 - 1) / 2;          /* floor already correct */

    if (k_end < k_start)
        return (seq_t)0;

    return (seq_t)(k_end - k_start + 1);
}

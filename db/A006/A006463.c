/*
 * A006463  Convolve natural numbers with characteristic function of
 *          triangular numbers.
 * https://oeis.org/A006463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Let n = T(m) + r where T(m) = m*(m+1)/2 and 0 <= r <= m; then
 *            a(n) = (1/3)*m*(m^2 + 3*r - 1)
 * Range:     u8 n <= 46, u16 n <= 1719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static uint64_t isqrt_u64(uint64_t x) {
    if (x == 0) return 0;
    uint64_t m = x;
    while (1) {
        uint64_t m_new = (m + x / m) / 2;
        if (m_new >= m) return m;
        m = m_new;
    }
}

seq_t a(idx_t n) {
    uint64_t discriminant = 1 + 8 * n;
    uint64_t sqrt_disc = isqrt_u64(discriminant);
    uint64_t m = (sqrt_disc - 1) / 2;
    
    uint64_t triangular = m * (m + 1) / 2;
    uint64_t r = n - triangular;
    
    uint64_t m2 = m * m;
    uint64_t numerator = m * (m2 + 3 * r - 1);
    uint64_t result_u64 = numerator / 3;
    
    return (seq_t)result_u64;
}

/*
 * A001460  a(n) = (5*n)!/((2*n)!*(n!)^3).
 * https://oeis.org/A001460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n,n) * binomial(4*n,n) * binomial(5*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t gcd_idx(idx_t x, idx_t y) {
    while (y) {
        idx_t t = y;
        y = x % y;
        x = t;
    }
    return x;
}

static seq_t compute_binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    
    seq_t result = 1;
    for (idx_t i = 1; i <= k; i++) {
        idx_t g = gcd_idx(n - k + i, i);
        result = result * ((n - k + i) / g) / (i / g);
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t b3n = compute_binom(3*n, n);
    seq_t b4n = compute_binom(4*n, n);
    seq_t b5n = compute_binom(5*n, n);
    
    return b3n * b4n * b5n;
}

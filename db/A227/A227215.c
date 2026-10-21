/*
 * A227215  Smallest sum of the three perpendicular integer sides of a
 *          rectangular parallelepiped of volume n.
 * https://oeis.org/A227215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum a+b+c such that a*b*c = n, a,b,c ≥ 1, a≤b≤c
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The maximum possible sum is n + 1 + 1 (when a = 1, b = 1, c = n) */
    idx_t best = n + 2;
    for (idx_t a = 1; a * a * a <= n; ++a) {
        if (n % a != 0) continue;
        idx_t n1 = n / a;
        for (idx_t b = a; b * b <= n1; ++b) {
            if (n1 % b != 0) continue;
            idx_t c = n1 / b;
            idx_t sum = a + b + c;
            if (sum < best) best = sum;
        }
    }
    return (seq_t)best;
}

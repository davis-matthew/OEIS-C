/*
 * A053618  a(n) = ceiling(binomial(n,4)/n).
 * https://oeis.org/A053618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(binomial(n,4)/n) = ceil((n-1)*(n-2)*(n-3)/24)
 * Range:     u8 n <= 20, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    unsigned long long x = (unsigned long long)(n - 1);
    unsigned long long y = (unsigned long long)(n - 2);
    unsigned long long z = (unsigned long long)(n - 3);
    /* compute ceil((x*y*z)/24) in 64‑bit intermediate to avoid overflow */
    unsigned long long prod = x * y * z + 23ULL; /* +23 for ceil */
    return (seq_t)(prod / 24ULL);
}

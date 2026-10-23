/*
 * A180415  a(n) = (n^3 - 3n^2 + 14n - 6)/6.
 * https://oeis.org/A180415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - 3*n^2 + 14*n - 6)/6 Equivalently a(n) = C(n-1,3) +
 *            n*(n+1)/2 (for n ≥ 4)
 * Range:     u8 n <= 12, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms directly to avoid negative indices */
    if (n < 4) {
        static const seq_t first_three[3] = {1, 3, 6};
        return first_three[n - 1];
    }

    /* compute C(n-1,3) safely, keeping the product small */
    seq_t a1 = (seq_t)(n - 1);
    seq_t a2 = (seq_t)(n - 2);
    seq_t a3 = (seq_t)(n - 3);

    /* divide by 2 first (any factor is even) */
    if ((a1 & (seq_t)1) == 0) a1 >>= 1;
    else if ((a2 & (seq_t)1) == 0) a2 >>= 1;
    else a3 >>= 1;

    /* divide by 3 next (any factor is divisible by 3) */
    if (a1 % 3 == 0) a1 /= 3;
    else if (a2 % 3 == 0) a2 /= 3;
    else a3 /= 3;

    /* product of the three factors gives C(n-1,3) */
    seq_t comb = a1 * a2;
    comb *= a3;

    /* compute n*(n+1)/2 safely */
    seq_t tri;
    if (n & 1ULL)          /* n is odd: divide (n+1) first */
        tri = ((seq_t)(n + 1) / 2) * (seq_t)n;
    else                  /* n is even: divide n first */
        tri = ((seq_t)n / 2) * (seq_t)(n + 1);

    return comb + tri;
}

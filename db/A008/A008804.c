/*
 * A008804  Expansion of 1/((1-x)^2*(1-x^2)*(1-x^4)).
 * https://oeis.org/A008804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   for n >= 8, a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - 2*a(n-5) +
 *            2*a(n-7) - a(n-8) with initial values a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=6, a(4)=10, a(5)=14, a(6)=20, a(7)=26 (expansion of
 *            1/((1-x)^2*(1-x^2)*(1-x^4)) )
 * Range:     u8 n <= 19, u16 n <= 142, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial eight values */
    static const seq_t init[8] = {1, 2, 4, 6, 10, 14, 20, 26};

    if (n < 8)
        return init[n];

    /* Ring buffer to keep the last eight computed values */
    seq_t ring[8];
    for (idx_t i = 0; i < 8; ++i)
        ring[i] = init[i];

    /* Compute successive terms up to n, applying the recurrence */
    for (idx_t i = 8; i <= n; ++i) {
        /* Indices (mod 8) of the required previous terms */
        seq_t a1 = ring[(i - 1) & 7];
        seq_t a3 = ring[(i - 3) & 7];
        seq_t a4 = ring[(i - 4) & 7];
        seq_t a5 = ring[(i - 5) & 7];
        seq_t a7 = ring[(i - 7) & 7];
        seq_t a8 = ring[(i - 8) & 7];

        /* Compute a(i) = 2*a1 + 2*a4 + 2*a7 - (2*a3 + 2*a5 + a8) */
        seq_t sum_pos = (seq_t)2 * a1 + (seq_t)2 * a4 + (seq_t)2 * a7;
        seq_t sum_neg = (seq_t)2 * a3 + (seq_t)2 * a5 + a8;
        seq_t next    = sum_pos - sum_neg;

        ring[i & 7] = next;
    }

    return ring[n & 7];
}

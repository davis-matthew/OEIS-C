/*
 * A156684  The number of primitive Pythagorean triples A^2+B^2=C^2 with 0 < A
 *          < B < C and gcd(A,B)=1, and both legs less than n.
 * https://oeis.org/A156684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1597, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * The function counts primitive Pythagorean triples a^2 + b^2 = c^2 with
 * 0 < a < b < c, gcd(a, b) = 1, and both legs a and b are less than n.
 * It uses Euclid's formula: for coprime m > k, opposite parity,
 * a = m^2 - k^2,  b = 2*m*k.
 * For each such pair (m, k) the function checks whether both legs are
 * < n and increments the counter.  The algorithm runs in O(n log n)
 * time with a small constant and is fast for n up to about 10 000.
 */

#include <stdint.h>          /* may be needed for type definitions */

/* Euclid's formula generates each primitive triple exactly once.  */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Count primitive triples with legs less than n. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n < 2) return 0;                /* nothing can exist */

    /* For 2*m*k < n, we need m < n/2.  */
    idx_t max_m = (n - 1) / 2;           /* integer division */
    for (idx_t m = 2; m <= max_m; ++m) {
        /* maximal k such that 2*m*k < n  */
        idx_t k_max = (n - 1) / (2 * m);
        if (k_max == 0) continue;       /* no k possible for this m */

        /* k must also be < m */
        if (k_max > m - 1) k_max = m - 1;

        idx_t k;
        for (k = 1; k <= k_max; ++k) {
            /* Parity condition: one of m, k must be even.  */
            if (((m - k) & 1) == 0) continue;
            /* Coprime condition.  */
            if (gcd_idx(m, k) != 1) continue;

            /* Compute the two legs.  All arithmetic here fits in
             * seq_t, which is at least 64 bits for the ranges used.  */
            seq_t a_leg = (seq_t)m * (seq_t)m - (seq_t)k * (seq_t)k;
            seq_t b_leg = (seq_t)2 * (seq_t)m * (seq_t)k;

            if (a_leg < n && b_leg < n) {
                ++count;                /* a primitive triple found */
            }
        }
    }
    return count;
}

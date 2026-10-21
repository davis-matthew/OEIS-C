/*
 * A056296  Number of n-bead necklace structures using exactly three different
 *          colored beads.
 * https://oeis.org/A056296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{d|n} φ(d) * * ( [d≡0 (mod 6)] * (S₂(n/d+2, 3)
 *            – S₂(n/d+1, 3)) * * + [d≡3 (mod 6)] * (S₂(n/d+2, 3) – 3
 *            S₂(n/d+1, 3) + 3 S₂(n/d, 3)) * * + [d≡2 or 4 (mod 6)] * (2
 *            S₂(n/d+1, 3) – 2 S₂(n/d, 3)) * * + [d≡1 or 5 (mod 6)] * S₂(n/d,
 *            3) ); * where S₂(m, k) are Stirling numbers of the sec...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

/* Stirling numbers of the second kind S₂(n,3) as a closed formula. */
static seq_t Stirling2_3(idx_t n)
{
    if (n < 3) return 0;
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t temp = pow3 - pow2 * (seq_t)3 + (seq_t)3;
    return temp / (seq_t)6;
}

/* Term contributed by a single divisor d of n. */
static seq_t term_for_divisor(idx_t d, idx_t n)
{
    idx_t m = n / d;
    seq_t phi_d = phi(d);

    seq_t s2_m  = 0, s2_m1 = 0, s2_m2 = 0;
    if (m      >= 3) s2_m  = Stirling2_3(m);
    if (m + 1  >= 3) s2_m1 = Stirling2_3(m + 1);
    if (m + 2  >= 3) s2_m2 = Stirling2_3(m + 2);

    seq_t term = 0;
    switch (d % 6) {
    case 0:
        term = s2_m2 - s2_m1;
        break;
    case 3:
        term = s2_m2 - (seq_t)3 * s2_m1 + (seq_t)3 * s2_m;
        break;
    case 2:
    case 4:
        term = (seq_t)2 * s2_m1 - (seq_t)2 * s2_m;
        break;
    case 1:
    case 5:
        term = s2_m;
        break;
    }
    return phi_d * term;
}

/* Main function as required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not used, offset starts at 1 */
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sum += term_for_divisor(i, n);
            idx_t d2 = n / i;
            if (d2 != i) sum += term_for_divisor(d2, n);
        }
    }
    return sum / (seq_t)n;
}

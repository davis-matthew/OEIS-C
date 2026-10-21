/*
 * A053158  Sum of n and its cototient function value (A051953): a(n) = 2*n -
 *          phi(n), where phi is Euler phi.
 * https://oeis.org/A053158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - φ(n) (σ is Euler's totient)
 * Range:     u8 n <= 149, u16 n <= 9914, u32 n <= 9914, u64 n <= 9914,
 *            u128 n <= 9914, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute φ(n) with trial division */
    idx_t m = n;
    seq_t phi_val = (seq_t)n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            phi_val -= phi_val / (idx_t)p;
        }
    }
    if (m > 1) {
        phi_val -= phi_val / m;
    }

    return ((seq_t)n * 2) - phi_val;
}

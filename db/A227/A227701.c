/*
 * A227701  The Wiener index of the nanostar dendrimer defined pictorially in
 *          Fig. 1 of the Iranmanesh et al. reference.
 * https://oeis.org/A227701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22323 + 2^n*(9018 + 2664*n) + 4^n*(216 + 432*n)
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 25, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term with offset 1; for n < 1 behaviour undefined, return 0 */
    if (n == (idx_t)0) return (seq_t)0;

    seq_t power2 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n; ++i) {
        power2 *= (seq_t)2;
    }

    seq_t power4 = power2 * power2;   /* 4^n = (2^n)^2 */

    seq_t term = (seq_t)22323;

    seq_t coeff1 = (seq_t)9018 + (seq_t)2664 * (seq_t)n;
    term += power2 * coeff1;

    seq_t coeff2 = (seq_t)216 + (seq_t)432 * (seq_t)n;
    term += power4 * coeff2;

    return term;
}

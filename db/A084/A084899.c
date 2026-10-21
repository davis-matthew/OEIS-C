/*
 * A084899  Binomial transform of heptagonal numbers A000566.
 * https://oeis.org/A084899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n - 2)*n*(5n - 1)/2 (for n >= 1), a(0) = 0. Derived
 *            recurrence: a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3) (n >= 3)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;

    /* Use recurrence a_n = 6*a_{n-1} - 12*a_{n-2} + 8*a_{n-3}. */
    seq_t a0 = 0;         /* a_{i-3} */
    seq_t a1 = 1;         /* a_{i-2} */
    seq_t a2 = 9;         /* a_{i-1} */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute in 128‑bit to prevent overflow for 64‑bit seq_t. */
        ai = (seq_t)((unsigned __int128)6 * a2 -
                     (unsigned __int128)12 * a1 +
                     (unsigned __int128)8 * a0);
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}

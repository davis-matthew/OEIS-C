/*
 * A017900  Expansion of 1/(1 - x^6 - x^7 - x^8 - ...).
 * https://oeis.org/A017900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = a(2) = a(3) = a(4) = a(5) = 0; For n ≥ 6, a(n)
 *            = a(n‑1) + a(n‑6) (OEIS A017900)
 * Range:     u8 n <= 31, u16 n <= 53, u32 n <= 97, u64 n <= 185,
 *            u128 n <= 362, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n <= 5) return (seq_t)0;

    /* store the last six values a(n‑5) … a(n) */
    seq_t last[6] = {
        (seq_t)1,   /* a(0) */
        (seq_t)0,   /* a(1) */
        (seq_t)0,   /* a(2) */
        (seq_t)0,   /* a(3) */
        (seq_t)0,   /* a(4) */
        (seq_t)0    /* a(5) */
    };

    for (idx_t i = 6; i <= n; ++i) {
        seq_t cur = last[(i - 1) % 6] + last[(i - 6) % 6];
        last[i % 6] = cur;
    }
    return last[n % 6];
}

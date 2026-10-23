/*
 * A041126  Numerators of continued fraction convergents to sqrt(72).
 * https://oeis.org/A041126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041126: Numerators of continued‑fraction convergents to sqrt(72)
   Recurrence: a(0)=8, a(1)=17, for n≥2
   a(n) = ((n%2==0) ? 16 : 2) * a(n‑1) + a(n‑2)   (periodic coefficients 16,2) */

seq_t a(idx_t n) {
    if (n == 0) return 8;
    if (n == 1) return 17;

    seq_t prev2 = 8;   /* a(0) */
    seq_t prev1 = 17;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = (i & 1) ? 2 : 16;   /* odd i → 2, even i → 16 */
        seq_t curr = mult * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

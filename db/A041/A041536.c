/*
 * A041536  Numerators of continued fraction convergents to sqrt(285).
 * https://oeis.org/A041536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(285).
   Recurrence:
       a(0) = 16
       a(1) = 17
       For n ≥ 2:
           a(n) = coeff[(n-1) % 6] * a(n-1) + a(n-2)
   where coeff = {1, 7, 2, 7, 1, 32} corresponds to the periodic continued
   fraction expansion [16; (1,7,2,7,1,32)] that repeats. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)16;
    if (n == 1) return (seq_t)17;

    static const idx_t coeff[6] = {1, 7, 2, 7, 1, 32};

    seq_t prev2 = (seq_t)16; /* a(0) */
    seq_t prev1 = (seq_t)17; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t c = (seq_t)coeff[(i - 1) % 6];
        seq_t cur = c * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

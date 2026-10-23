/*
 * A041085  Denominators of continued fraction convergents to sqrt(50).
 * https://oeis.org/A041085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, frac
 * Formula:   a(n) = 14*a(n-1) + a(n-2), a(0) = 1, a(1) = 14
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is computed iteratively, so no dynamic memory is needed. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)14;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 * (seq_t)14 + prev2;  /* a(i) = 14*a(i-1)+a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

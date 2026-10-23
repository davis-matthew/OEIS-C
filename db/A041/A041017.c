/*
 * A041017  Denominators of continued fraction convergents to sqrt(12).
 * https://oeis.org/A041017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) =
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0) = 1, a(1) = 2, */
/*   a(n) = 6 * a(n-1) + a(n-2)   if n is even, */
/*          2 * a(n-1) + a(n-2)   if n is odd. */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 2;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur;
        if ((i & 1) == 0)           /* even i */
            cur = (seq_t)6 * prev1 + prev2;
        else                        /* odd i */
            cur = (seq_t)2 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

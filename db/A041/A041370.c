/*
 * A041370  Numerators of continued fraction convergents to sqrt(200).
 * https://oeis.org/A041370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=14; for n>=1, a(n)=c(n)*a(n-1)+a(n-2) * where c(n)=7 if n
 *            is odd, 28 if n is even. * This recurrence follows from the
 *            continued fraction of sqrt(200) * with periodic part [7,28]
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0) return 14;
    if (n == 1) return 99;

    seq_t prev2 = 0; /* p_{-2} */
    seq_t prev1 = 1; /* p_{-1} */
    seq_t curr  = 0; /* current numerator */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0)          coeff = 14;
        else if (i & 1)      coeff = 7;
        else                 coeff = 28;

        curr = coeff * prev1 + prev2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}

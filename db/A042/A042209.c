/*
 * A042209  Denominators of continued fraction convergents to sqrt(630).
 * https://oeis.org/A042209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 10.
   For n ≥ 2,
     if n is odd  → a(n) = 10 * a(n-1) + a(n-2)
     if n is even → a(n) = 50 * a(n-1) + a(n-2) */
seq_t a(idx_t n)
{
    seq_t prev2 = (seq_t)1;          /* q_{-2} */
    seq_t prev1 = (seq_t)0;          /* q_{-1} */
    seq_t cur;

    for (idx_t i = 0; ; ++i)
    {
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)25;
        else if (i & 1)
            coeff = (seq_t)10;   /* odd i → 10 */
        else
            coeff = (seq_t)50;   /* even i → 50 */

        cur = coeff * prev1 + prev2;

        if (i == n)
            return cur;

        prev2 = prev1;
        prev1 = cur;
    }
}

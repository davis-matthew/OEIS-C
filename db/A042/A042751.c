/*
 * A042751  Denominators of continued fraction convergents to sqrt(906).
 * https://oeis.org/A042751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=10;
   for n>=2: a(n) = coeff(n)*a(n-1)+a(n-2),
   coeff(n) = 60 if n is even, 10 if n is odd. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)10;  /* a(n-1) */
    seq_t cur   = 0;          /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff = (i & 1) ? (seq_t)10 : (seq_t)60; /* odd →10, even→60 */
        cur = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}

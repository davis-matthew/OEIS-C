/*
 * A041682  Numerators of continued fraction convergents to sqrt(360).
 * https://oeis.org/A041682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continuants for sqrt(360): recurrence
   a(0) = 18, a(1) = 19;
   for n >= 2  a(n) = coeff(n) * a(n-1) + a(n-2)
   where coeff(n) = 36 when n is even, otherwise 1.
   The values are the numerators of the convergents to sqrt(360). */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)18;
    if (n == 1ULL)
        return (seq_t)19;

    /* coefficients for the recurrence */
    const seq_t ONE = (seq_t)1;
    const seq_t THIRTY_SIX = (seq_t)36;

    /* previous two terms: a(n-2), a(n-1) */
    seq_t prev2 = (seq_t)18;  /* a(0) */
    seq_t prev1 = (seq_t)19;  /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2ULL; i <= n; ++i)
    {
        const seq_t coeff = ( (i & 1ULL) == 0ULL ) ? THIRTY_SIX : ONE;
        cur = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

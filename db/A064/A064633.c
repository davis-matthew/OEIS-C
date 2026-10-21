/*
 * A064633  a(n) = 3^n*n!*(n+2)!/2!.
 * https://oeis.org/A064633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * n! * (n+2)! / 2!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 3^n * (n!)^2 * (n+1)(n+2) / 2  */
seq_t a(idx_t n)
{
    /* term (n+1)(n+2)/2, integer for all n */
    seq_t factor = (seq_t)(n + 1);
    factor *= (seq_t)(n + 2);
    factor /= (seq_t)2;          /* now factor = (n+1)(n+2)/2 */

    /* compute 3^n */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;

    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* term = factor * pow3 * fact^2 */
    seq_t result = fact * fact;   /* (n!)^2 */
    result *= pow3;              /* *3^n */
    result *= factor;            /* * (n+1)(n+2)/2 */

    return result;
}

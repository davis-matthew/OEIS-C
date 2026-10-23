/*
 * A185009  Row sums of A051949 (differences of factorial numbers), seen as a
 *          triangle.
 * https://oeis.org/A185009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n+1)*n! - Sum_{i=1..n-1} i!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: a(1) = 0 */
    if (n <= 1) return (seq_t)0;

    /* fact holds (k-1)! during iteration for k = 2..n   */
    seq_t fact = (seq_t)1;   /* 1!  */
    seq_t result = (seq_t)0; /* a(1) */

    for (idx_t k = 2; k <= n; ++k)
    {
        idx_t m = k - 1;  /* previous index */

        /* poly = m^3 + 2*m^2 + 2*m   */
        seq_t mm = (seq_t)m;
        seq_t mm2 = mm * mm;          /* m^2   */
        seq_t poly = mm2 * mm + ((seq_t)2) * mm2 + ((seq_t)2) * mm;

        /* Add the increment to result */
        result = result + fact * poly;

        /* Prepare fact for next step: fact = fact * k = k!  */
        fact = fact * ((seq_t)k);
    }

    return result;
}

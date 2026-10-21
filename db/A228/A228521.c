/*
 * A228521  x-values in the solution to the Pell equation x^2 - 29*y^2 = -1.
 * https://oeis.org/A228521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19602*a(n-1) - a(n-2), a(1) = 70, a(2) = 1372210
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is applied for all n ≥ 1.  For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Handle the two known initial values directly. */
    if (n == 1) {
        return (seq_t)70;
    }
    if (n == 2) {
        return (seq_t)1372210;
    }

    /* Iteratively compute the sequence up to the requested index. */
    seq_t a1 = (seq_t)70;          /* a(1)   */
    seq_t a2 = (seq_t)1372210;     /* a(2)   */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t an = (seq_t)19602 * a2 - a1;
        a1 = a2;
        a2 = an;
    }
    return a2; /* a(n) is now stored in a2 */
}

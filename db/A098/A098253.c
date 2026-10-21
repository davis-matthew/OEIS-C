/*
 * A098253  First differences of Chebyshev polynomials S(n,363) = A098251(n)
 *          with Diophantine property.
 * https://oeis.org/A098253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 362, a(n) = 363*a(n-1) - a(n-2) (n ≥ 2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)362;

    /* Compute the recurrence iteratively.  This runs in O(n) time and
       uses only the arithmetic operators that are guaranteed to be
       defined for seq_t. */
    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)362; /* a(n-1) */
    seq_t an;   /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)363 * a1 - a0;
        a0 = a1;
        a1 = an;
    }

    return a1;
}

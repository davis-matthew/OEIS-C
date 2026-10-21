/*
 * A220034  Number of 4 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 4 X n array.
 * https://oeis.org/A220034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 3*n^2 + 86*n - 270)/6 for n ≥ 6, a(1)=5, a(2)=8,
 *            a(3)=15, a(4)=34, a(5)=61
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle values below the offset (not used, but harmless) */
    if (n == 0) return 0;

    /* Initial terms (n = 1..5). A simple table is more efficient than
       a switch when called repeatedly. */
    static const seq_t init[] = {5, 8, 15, 34, 61};

    if (n <= 5)
        return init[n - 1];

    /* Compute a(n) for n ≥ 6 using the closed‑form polynomial.  The
       numerator is always divisible by 6, so integer division yields an
       exact result. */
    seq_t nn   = (seq_t)n;            /* n */
    seq_t n2   = nn * nn;             /* n^2 */
    seq_t n3   = n2 * nn;             /* n^3 */
    seq_t num  = n3 + 3 * n2 + 86 * nn - 270;
    return num / 6;
}

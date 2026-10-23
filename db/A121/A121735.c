/*
 * A121735  Real term generated from a complex product operation.
 * https://oeis.org/A121735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = n! if 3 divides n, otherwise a(n) =
 *            -n!/2
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A121735 for a given index n. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for n < 1, return 0 */

    /* Compute n! */
    seq_t res = 1;
    for (idx_t k = 2; k <= n; ++k)
        res *= (seq_t)k;

    /* If n > 1 and n is not a multiple of 3, res = -n!/2 */
    if (n > 1 && (n % 3) != 0) {
        res /= 2;                    /* n! is even for n > 1 */
        res = -res;                  /* negate */
    }
    return res;
}

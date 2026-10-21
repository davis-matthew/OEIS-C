/*
 * A052182  Determinant of n X n matrix whose rows are cyclic permutations of
 *          1..n.
 * https://oeis.org/A052182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, nice
 * Formula:   a(n) = (-1)^(n-1) * n^(n-2) * (n^2 + n)/2 for n>=1, a(0)=1
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Compute n^(n-2) using binary exponentiation */
    idx_t e = n - 2;              /* exponent (non‑negative because n>=2) */
    seq_t base = (seq_t)n;
    seq_t power = (seq_t)1;

    while (e > 0)
    {
        if (e & 1)
            power = power * base;
        base = base * base;
        e >>= 1;
    }

    /* Compute (n^2 + n)/2 = n*(n+1)/2  */
    seq_t tri = (seq_t)n * (seq_t)(n + 1);
    tri /= 2;                /* integer division, exact */

    /* Multiply the components */
    seq_t result = power * tri;

    /* Apply the sign factor (-1)^(n-1) */
    if ((n - 1) & 1)
        result = -result;

    return result;
}

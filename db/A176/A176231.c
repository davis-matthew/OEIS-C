/*
 * A176231  Coefficient array of orthogonal polynomials whose moment sequence
 *          is the double factorial numbers A001147.
 * https://oeis.org/A176231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the n‑th entry of the row‑major list of the triangle
 *            T(r,c) defined by T(r,c) = (-1)^(r-c) · (2r)! / ( (2c)! · (r-c)!
 *            · 2^(r-c) )
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 27, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r and column c such that n lies in that position
       (flat index).  The number of entries up to row r-1 is r*(r+1)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t c = n - r * (r + 1) / 2;
    idx_t d = r - c;               /* d = r - c  (= number of steps from last column) */

    /* Compute the unsigned part of T(r,c). */
    seq_t val = 1;

    /* Numerator: product of integers from (2c+1) up to (2r). */
    for (idx_t j = 2 * c + 1; j <= 2 * r; ++j)
        val *= (seq_t)j;

    /* Divide by d! (factorial of the column offset). */
    for (idx_t i = 1; i <= d; ++i)
        val /= (seq_t)i;

    /* Divide by 2^d. */
    for (idx_t i = 1; i <= d; ++i)
        val /= (seq_t)2;

    /* Apply sign (-1)^d. */
    if (d & 1u)
        val = -val;

    return val;
}

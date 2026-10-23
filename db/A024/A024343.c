/*
 * A024343  Expansion of e.g.f. sin(x^2) in powers of x^(4*n + 2).
 * https://oeis.org/A024343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (4*n+2)! / (2*n+1)!
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 3, i64 n <= 6, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = (-1)^n * ∏_{k=2*n+2}^{4*n+2} k   for n ≥ 0 */

/* External linkage function required by the build system. */
seq_t a(idx_t n)
{
    /* Compute the product (2*n+2)*(2*n+3)*…*(4*n+2). */
    seq_t prod = 1;
    idx_t start = 2 * n + 2;      /* first factor */
    idx_t end   = 4 * n + 2;      /* last factor  */

    for (idx_t k = start; k <= end; ++k) {
        prod *= (seq_t)k;
    }

    /* Apply the sign factor (-1)^n. */
    if (n & 1)
        prod = -prod;

    return prod;
}

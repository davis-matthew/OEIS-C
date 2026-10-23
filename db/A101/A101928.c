/*
 * A101928  E.g.f. cos(arcsinh(x)) = sin(arccosh(x)) (even powers only).
 * https://oeis.org/A101928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n+1) * prod_{k=1}^{n-2} (4*k^2 + 1) with a(1) = 1,
 *            a(2) = -1
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-1;

    seq_t result = (n & 1) ? (seq_t)1 : (seq_t)-1;   /* (-1)^(n+1) */

    /* Multiply by (4*k^2 + 1) for k = 1 .. n-2 */
    for (idx_t k = 1; k < n - 1; ++k)
    {
        /* Compute 4*k*k + 1 in 64‑bit to avoid intermediate overflow */
        uint64_t kk  = k * k;          /* k^2 */
        uint64_t t   = 4ULL * kk + 1;  /* 4*k^2 + 1 */

        result *= (seq_t)t;
    }
    return result;
}

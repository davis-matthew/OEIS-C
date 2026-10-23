/*
 * A175628  a(2*n+1) = A005563(n). a(2*n) = A061037(n+1).
 * https://oeis.org/A175628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n-1)*(n+3)/4      when n is odd
 *             a(n) = (n^2 + 4*n - 12) * (37 + 27*(-1)^(n/2) + 6*cos((n+2)*π/4)) / 256
 *             when n is even.
 * Note: For even n, cos((n+2)*π/4) ∈ { -1, 0, 1 } depending on n mod 8,
 *       so the formula reduces to integer arithmetic only.
 */

seq_t a(idx_t n)
{
    /* odd n */
    if (n & 1U) {
        seq_t num = (seq_t)(n - 1U) * (seq_t)(n + 3U);
        return num / 4U;
    }

    /* even n */
    /* Compute A = n^2 + 4*n - 12 */
    seq_t A = (seq_t)n * (seq_t)n + 4U * (seq_t)n - 12U;

    /* Compute (-1)^(n/2) : e = 1 if (n/2) even, -1 otherwise */
    idx_t m = n >> 1U;          /* n/2 */
    int e = (m & 1U) ? -1 : 1;  /* (-1)^m */

    /* Compute cos((n+2)*π/4) via n mod 8 pattern */
    int rem8 = n & 7U;         /* n % 8 */
    int c = 0;
    if (rem8 == 2) c = -1;     /* cos((n+2)*π/4) = -1 */
    else if (rem8 == 6) c = 1; /* cos((n+2)*π/4) =  1 */

    /* B = 37 + 27*e + 6*c */
    int B_int = 37 + 27 * e + 6 * c;
    seq_t B = (seq_t)B_int;   /* B is positive for all even n */

    /* result = A * B / 256 */
    seq_t prod = A * B;
    return prod / 256U;
}

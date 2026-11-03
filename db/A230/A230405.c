/*
 * A230405  a(n) = A000217(A230404(n+1)); the first differences of A219650.
 * https://oeis.org/A230405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2563, u16 n <= 10079, u32 n <= 10079, u64 n <= 10079,
 *            u128 n <= 10079, big any n
 * Validated: standard, pass, 103 terms through n = 8908 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = T(z), where z is the number of trailing zeros in
 * the factorial-base representation of (2n+2).
 *
 * Let m = 2n+2.  Compute the largest integer r such that r! divides m;
 * then z = r-1.  The term is the triangular number z*(z+1)/2.
 */

seq_t a(idx_t n)
{
    /* m = 2n + 2 */
    seq_t m = (seq_t)n;
    m = m * 2 + 2;

    seq_t fact = 1;          /* current factorial i! */
    idx_t r = 1;             /* largest i with i! | m */

    for (idx_t i = 2;; ++i) {
        fact = fact * (seq_t)i;
        if (m % fact == 0)
            r = i;
        else
            break;
    }

    idx_t z = r - 1;                     /* trailing zeros count */
    seq_t res = (seq_t)z * (z + 1) / 2;  /* triangular number */
    return res;
}

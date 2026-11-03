/*
 * A198789  Array T(n,k) read by antidiagonals: Last survivor positions in
 *          Josephus problem for n numbers and a count of k, n >= 1, k >= 1.
 * https://oeis.org/A198789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(i,j) where T is the Josephus survivor. For n ≥ 1,
 *            decode the term index n into antidiagonal coordinates (i,j) of
 *            the infinite array T(i,j) defined by T(1,k) = 1; T(i,k) =
 *            ((T(i-1,k) + k - 1) mod i) + 1 for i > 1. Then return T(i,j)
 * Range:     u8 n <= 10011, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 107 terms through n = 9094 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n=0; safe fallback */

    /* Find the diagonal S such that S*(S-1)/2 ≥ n.
       Use incremental addition to avoid overflow. */
    idx_t S   = 2;         /* current diagonal sum (n + k - 1) */
    idx_t cur = 1;         /* total terms up to diagonal S  */
    while (cur < n) {
        ++S;
        cur += S - 1;     /* add terms of the new diagonal */
    }

    /* Number of terms before this diagonal */
    idx_t prior = cur - (S - 1);
    /* Offset within the diagonal (1 … S-1) */
    idx_t t = n - prior;
    /* Coordinates in the array T(n,k) */
    idx_t i = t;              /* row index */
    idx_t k = S - t;          /* column index */

    /* Compute T(i,k) using the recurrence. */
    seq_t res = 1;            /* T(1,k) = 1 */
    for (idx_t j = 2; j <= i; ++j) {
        res = ((res + (seq_t)(k - 1)) % (seq_t)j) + 1;
    }
    return res;
}

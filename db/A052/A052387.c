/*
 * A052387  Number of 3 X n binary matrices such that any 2 rows have a common
 *          1, up to column permutations.
 * https://oeis.org/A052387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n^3 +22*n^2 +53*n +134)/5040
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow while using only
       integer arithmetic.  All operations are performed on the
       unsigned __int128 type and the final result is cast back to
       seq_t, which guarantees correctness when the result fits. */
    unsigned __int128 N = n;
    unsigned __int128 poly = N * N * N               /* n^3   */
                         + (unsigned __int128)22 * N * N /* 22 n^2 */
                         + (unsigned __int128)53 * N     /* 53 n   */
                         + 134;                         /* constant */
    unsigned __int128 prod = N * (N+1) * (N+2) * (N+3) * poly;
    prod /= 5040;
    return (seq_t)prod;
}

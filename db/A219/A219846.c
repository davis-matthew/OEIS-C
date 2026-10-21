/*
 * A219846  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..2 n X 2 array.
 * https://oeis.org/A219846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 2*n^3 + 23*n^2 - 2*n + 48) / 24
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the closed‑form expression for A219846.    */
seq_t a(idx_t n)
{
    /* Use a 128‑bit temporary type to avoid overflow for n up to 10 000.
       idx_t is at least 64‑bit, so casting to __int128 is safe. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 n2  = N * N;      /* n^2   */
    unsigned __int128 n3  = n2 * N;      /* n^3   */
    unsigned __int128 n4  = n3 * N;      /* n^4   */
    unsigned __int128 num = n4 + 2*n3 + 23*n2 + 48;  /* numerator parts */
    num -= 2 * N;                       /* subtract 2*n     */
    return (seq_t)(num / 24);
}

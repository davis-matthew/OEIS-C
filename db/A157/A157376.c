/*
 * A157376  a(n) = 6561*n^2 - 7732*n + 2278.
 * https://oeis.org/A157376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6561*n^2 - 7732*n + 2278. Offset: 1 (a(1) is the first
 *            term)
 * Range:     u8 none, u16 n <= 3, u32 n <= 809, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first. */
    seq_t ns = (seq_t)n;

    /* Compute 6561*n^2  -  7732*n  +  2278.
       All operations are performed in seq_t, which may be a
       built‑in unsigned integer or an arbitrary‑precision class. */
    seq_t result = 6561 * ns * ns;   /* 6561*n^2 */
    result -= 7732 * ns;             /* - 7732*n  */
    result += 2278;                  /* + 2278     */

    return result;
}

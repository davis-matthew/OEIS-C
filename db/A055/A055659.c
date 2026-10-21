/*
 * A055659  Number of (2,n)-partitions of a chain of length n^3.
 * https://oeis.org/A055659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(n-1)*(n^2+n-1)*(n^3-2*n+2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* seq_t aliases an unsigned integer type; idx_t is an unsigned 64-bit type. */
    /* Use 128‑bit arithmetic for safety, then cast to seq_t. This works for
       any seq_t that can be constructed from a 128‑bit integer (built‑in
       types or arbitrary‑precision classes). */
    if (n <= 1u) {
        return 0;
    }

    unsigned __int128 N = n;
    unsigned __int128 term =
        (N - 1u) *
        (N * N + N - 1u) *
        (N * N * N - 2u * N + 2u);

    term /= 2u;

    return (seq_t)term;
}

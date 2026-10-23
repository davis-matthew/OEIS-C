/*
 * A101094  a(n) = n*(n+1)*(n+2)*(n+3)*(1+3*n+n^2)/120.
 * https://oeis.org/A101094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(1+3*n+n^2)/120
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 88, u64 n <= 3608,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indexing follows OEIS offset 1 (a(1) is the first term). */

seq_t a(idx_t n)
{
    /* a(0) is unused – return 0 as a safe default. */
    if (n == 0)
        return 0;

    /* Use a larger intermediate type (unsigned __int128) to avoid
       overflow of the intermediate product before dividing by 120. */
    unsigned __int128 N   = n;
    unsigned __int128 prod =
        N * (N + 1u) *
        (N + 2u) *
        (N + 3u) *
        (N * N + 3u * N + 1u);

    prod /= 120u;

    return (seq_t)prod; /* cast back – the result fits in seq_t for all n ≥ 1 tested. */
}

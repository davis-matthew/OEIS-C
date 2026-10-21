/*
 * A219680  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and nonincreasing columns, 0..2 n X 2 array.
 * https://oeis.org/A219680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 6*n^3 + 47*n^2 - 42*n)/24 for n>2; a(1) = 3, a(2)
 *            = 4
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;

    /* 64‑bit intermediate arithmetic – safe for n up to ~122 500 */
    unsigned long long N  = (unsigned long long)n;
    unsigned long long N2 = N * N;
    unsigned long long N3 = N2 * N;
    unsigned long long N4 = N3 * N;

    unsigned long long num = N4 - 6ULL * N3 + 47ULL * N2 - 42ULL * N;
    return (seq_t)(num / 24ULL);
}

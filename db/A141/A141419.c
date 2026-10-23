/*
 * A141419  Triangle read by rows: T(n, k) = A000217(n) - A000217(n - k) with
 *          1 <= k <= n.
 * https://oeis.org/A141419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k*(2*r - k + 1)/2 where r = smallest integer with
 *            r(r+1)/2 >= n, k = n - r(r-1)/2
 * Range:     u8 n <= 270, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Protect against accidental zero index: it is unused. */
    if (n == 0) return 0;

    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = 1;
    while ( ((seq_t)r * (r + 1)) / 2 < n )
        ++r;

    /* Compute k = n - r(r-1)/2. */
    seq_t prev = ((seq_t)r * (r - 1)) / 2;
    idx_t k = (idx_t)(n - (idx_t)prev);

    /* Compute the triangle entry: k*(2*r - k + 1)/2. */
    seq_t res = ((seq_t)k * (((seq_t)2 * r) - (seq_t)k + 1)) / 2;
    return res;
}

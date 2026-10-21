/*
 * A094587  Triangle of permutation coefficients arranged with 1's on the
 *          diagonal. Also, triangle of permutations on n letters with exactly
 *          k+1 cycles and with the first k+1 letters in separate cycles.
 * https://oeis.org/A094587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = r! / j! where * r = floor((sqrt(8*n+1)-1)/2) * j = n -
 *            r*(r+1)/2 * This is the flattening of the triangle T(r,j)=r!/j!
 *            rowwise
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r that contains index n.
     * The index of the first element of row r is r*(r+1)/2.
     * We increment r until that index exceeds n. */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum <= n) {
        ++r;
        sum += r;   /* sum = r*(r+1)/2 after the increment */
    }
    --r;                 /* now r is the appropriate row */

    /* Compute j = position within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t j = n - start;

    /* The triangle entry T(r,j) = r! / j! = product of numbers j+1 .. r */
    seq_t result = 1;
    for (idx_t t = j + 1; t <= r; ++t) {
        result = result * (seq_t)t;
    }
    return result;
}

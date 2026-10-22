/*
 * A003988  Triangle with subscripts (1,1),(2,1),(1,2),(3,1),(2,2),(1,3), etc.
 *          in which entry (i,j) is [ i/j ].
 * https://oeis.org/A003988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   T(i,j) = floor(i/j), read by antidiagonals r = i+j-1; a(n) =
 *            T(r-p+1, p) where n = r(r-1)/2 + p, 1 <= p <= r
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) r++;

    idx_t pos = n - r * (r - 1) / 2;
    idx_t i = r - pos + 1;
    idx_t j = pos;

    return (seq_t)(i / j);
}

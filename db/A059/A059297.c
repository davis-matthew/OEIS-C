/*
 * A059297  Triangle of idempotent numbers binomial(n,k)*k^(n-k), version 1.
 * https://oeis.org/A059297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(r,c) * c^(r-c) where r =
 *            floor((sqrt(8*n+1)-1)/2) (row of the triangle of idempotent
 *            numbers T(n,k)), c = n - r*(r+1)/2 (column within that row).
 *            This is the row‑major flattening of that triangle
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 110, u64 n <= 284,
 *            u128 n <= 791, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       start is the index of the first element in row r. */
    idx_t r = 0;
    idx_t start = 0;   /* start = r*(r+1)/2 */

    /* Increment r while the next row still starts within index n. */
    while ((n - start) >= (r + 1)) {
        r++;
        start += r;
    }

    idx_t c = n - start;  /* column index within row r */

    /* Handle the trivial cases where k = 0. */
    if (c == 0) {
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    seq_t pow_val = big_pow((seq_t)c, (seq_t)(r - c));
    seq_t bin = binomial(r, c);
    return bin * pow_val;
}

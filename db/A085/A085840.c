/*
 * A085840  Triangle read by rows: T(n,m) = 4^m * (2*n+1)! / ( (2*n - 2*m +
 *          1)! * (2*m)! ), row n has n+1 terms.
 * https://oeis.org/A085840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where r = ⌊(√(8n+1)-1)/2⌋, c = n - r(r+1)/2 and
 *            T(r,c) = 4^c * binomial(2*r+1, 2*c) (OEIS A085840)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t pow4(idx_t exp) {
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= 4;
    return r;
}

static idx_t triangular_row(idx_t n) {
    /* Find largest r such that r(r+1)/2 <= n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n) {
    /* Determine row r and column c of the flattened triangle */
    idx_t r = triangular_row(n);
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;

    /* Compute the binomial coefficient and multiply by 4^c */
    seq_t val = binomial(2 * r + 1, 2 * c);
    val *= pow4(c);
    return val;
}

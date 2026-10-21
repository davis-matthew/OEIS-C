/*
 * A093422  In the triangle shown below the n-th row contains n rational
 *          numbers n/1, {n*(n-1)}/{n +(n-1)}, {(n)*(n-1)*(n-2)}/{n
 *          +(n-1)+(n-2)}, ..., the last term being 2*(n-1)!/(n+1). Sequence
 *          gives the numerators in each row.
 * https://oeis.org/A093422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 43, u32 n <= 102, u64 n <= 252,
 *            u128 n <= 628, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdbool.h>

/* 
 * Formula: a(n) is the numerator of the reduced fraction at position n
 * in the triangle defined in OEIS A093422.  
 * Each row r has r terms.  For column c (1‑indexed) in row r the unreduced
 * fraction is
 *   P = r·(r-1)·…·(r-c+1)
 *   S = r + (r-1) + … + (r-c+1)
 * The returned value is the numerator of P/S after simplifying by gcd(P,S).
 *
 * Implementation uses an on‑the‑fly cancellation algorithm to avoid
 * intermediate overflow.  For 64‑bit builds the numerator will never exceed
 * 64 bits in the cases that the sequence fits in 64 bits.
 */

seq_t a(idx_t n) {
    // Find row r such that r*(r+1)/2 >= n
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n) {
        ++r;
    }
    idx_t prev_total = (r - 1) * r / 2;
    idx_t col = n - prev_total;          // 1‑indexed column within the row

    if (col == 1) {                      // first term of the row
        return (seq_t)r;
    }

    // Compute denominator S = sum of the first 'col' terms of the row
    seq_t denom = (seq_t)col * ((seq_t)2 * r - col + 1) / 2;

    seq_t num = 1;                       // running numerator after cancellations
    for (idx_t j = 0; j < col; ++j) {
        seq_t factor = (seq_t)(r - j);
        seq_t g = gcd(factor, denom);    // cancel common factors with denom
        if (g != 1) {
            denom /= g;
            factor /= g;
        }
        num *= factor;                   // remaining factor contributes to numerator
    }

    return num;                          // denom should be 1 after loop
}

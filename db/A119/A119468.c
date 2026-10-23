/*
 * A119468  Triangle read by rows: T(n,k) = Sum_{j=0..n-k}
 *          binomial(n,2j)*binomial(n-2j,k).
 * https://oeis.org/A119468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, pow2, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the term obtained by reading the triangle 
 * T(n,k) by rows where 
 *     T(n,k) = binomial(n,k) * 2^(n-k-1)   (for n > k)
 *     T(n,n) = 1
 * The index n is mapped to a row r and column c with
 *     r*(r+1)/2 <= n < (r+1)*(r+2)/2
 * and c = n - r*(r+1)/2. 
 */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* Determine row r */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    /* Compute column c */
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;

    /* If at diagonal return 1 */
    if (c == r) {
        return seq_one();
    }

    /* Compute T(r,c) = binomial(r,c) * 2^(r-c-1) */
    seq_t bin = binomial(r, c);
    seq_t pow = pow2(r - c - 1);
    return seq_mul(bin, pow);
}

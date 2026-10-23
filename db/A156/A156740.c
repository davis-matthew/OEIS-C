/*
 * A156740  Triangle T(n, k, m) = round( Product_{j=0..m} binomial(2*(n+j),
 *          2*(k+j))/binomial( 2*(n-k+j), 2*j) ), where m = 7, read by rows.
 * https://oeis.org/A156740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 48, u128 n <= 125,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth element of the triangle T(n,k,7) read by rows.
   For index n: let row r be the largest integer with r(r+1)/2 ≤ n,
   which equals floor((√(1+8n)-1)/2).  The column is k = n - r(r+1)/2.
   Then
          a(n) = round( ∏_{j=0..7} binom(2(r+j), 2(k+j)) / binom(2(r-k+j), 2j) ).
*/
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/binom.h"
#include "../utils/gcd.h"

/* compute a single T(r,k,7) value */
static inline seq_t compute_T(idx_t r, idx_t k)
{
    const idx_t M = 7;
    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;

    for (idx_t j = 0; j <= M; ++j)
    {
        idx_t a1 = 2 * (r + j);
        idx_t b1 = 2 * (k + j);
        seq_t bin1 = binom(a1, b1);

        idx_t a2 = 2 * (r - k + j);
        idx_t b2 = 2 * j;
        seq_t bin2 = binom(a2, b2);

        /* reduce bin1 with current denominator */
        seq_t g1 = gcd(bin1, den);
        if (g1 > (seq_t)1) {
            bin1 /= g1;
            den   /= g1;
        }

        /* reduce bin2 with current numerator */
        seq_t g2 = gcd(num, bin2);
        if (g2 > (seq_t)1) {
            num  /= g2;
            bin2  /= g2;
        }

        num *= bin1;
        den *= bin2;

        /* final reduction */
        seq_t g3 = gcd(num, den);
        if (g3 > (seq_t)1) {
            num /= g3;
            den /= g3;
        }
    }

    /* round the resulting fraction: (num + den/2) / den */
    return (num + den / (seq_t)2) / den;
}

/* deterministic mapping from flattened index n to (row r, col k) */
static inline idx_t row_of_index(idx_t n)
{
    /* approximate row using floating point sqrt, then correct */
    double t = sqrt((double)1 + 8.0 * (double)n);
    idx_t r = (idx_t)((t - 1.0) * 0.5);

    /* fine‑tune to guarantee r(r+1)/2 <= n < (r+1)(r+2)/2 */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    return r;
}

/* public sequence function */
seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t k = n - r * (r + 1) / 2;
    return compute_T(r, k);
}

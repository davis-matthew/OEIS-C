/*
 * A079508  Triangle T(n,k) (n >= 2, k >= 1) of Raney numbers read by rows.
 * https://oeis.org/A079508
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 115, u32 n <= 342, u64 n <= 1112,
 *            u128 n <= 3974, big any n
 * Helpers:   gcd, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) from OEIS A079508.  
 *
 *  The sequence is obtained by reading the triangle
 *
 *      T(n,k) = binomial(k, n-k) * binomial(n, k+1) / k
 *
 *  row n starts at index a(n) = 1 + n(n-1)/2 :  a(2)=1.
 *
 *  For a given index n (n≥2) we find the row and column
 *  from the offset, compute the entry by cancelling factors
 *  to avoid overflow, and return it in seq_t.
 *
 *  This implementation keeps all intermediate values in seq_t
 *  and performs prime‑factor cancellation, so it works
 *  correctly for 64‑bit seq_t and any arbitrary‑precision
 *  seq_t.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/int_sqrt.h"

#define MAX_ARRAY 200   /* sufficient for n≤10000 */

seq_t a(idx_t n)
{
    /* a(2) is the first term; nothing is defined for n<2. */
    if (n < 2) return 0;

    /* Convert the linear index to row and column in the triangle. */
    idx_t t   = n - 1;                 /* 0‑based position inside the infinite triangle */
    idx_t m   = 1 + 8 * t;             /* 1 + 8t  for quadratic formula */
    idx_t s   = int_sqrt(m);           /* floor sqrt(1+8t)   */
    idx_t row = (1 + s) / 2;           /* floor approximation */
    /* correct to ceil if necessary */
    if (row * (row - 1) / 2 < t) row++;

    idx_t prev = (row - 1) * (row - 2) / 2; /* elements before this row */
    idx_t k    = t - prev;                  /* 1‑based column in row */

    /* The triangle contains zeros wherever k < ceil(row/2). */
    if (row - k > k) return 0;

    /* Build lists of numerators and denominators of the product.
     *   T(row,k) = (∏(row-k+1 … row)) * (∏(k - (row-k) + 1 … k))
     *          ---------------------------------------
     *          (∏1 … (row-k)) * (∏1 … (k+1)) * k
     * The arrays are dimensioned well above the maximum size needed
     * for n up to 10000 (row<150).          */
    int t1 = row - k;          /* number of terms in the first binomial */
    int t2 = k + 1;            /* number of terms in the second binomial */
    int num_count = t1 + t2;   /* numerators */
    int den_count = num_count + 1; /* denominators (include the final k) */

    seq_t num[MAX_ARRAY];
    seq_t den[MAX_ARRAY];
    int ni = 0, di = 0;

    /* Numerators from binomial(row,k+1) */
    for (int j = 1; j <= t2; ++j)
        num[ni++] = (seq_t)(row - t2 + j);

    /* Numerators from binomial(k,row-k) */
    for (int i = 1; i <= t1; ++i)
        num[ni++] = (seq_t)(k - t1 + i);

    /* Denominators from binomial(row,k+1) */
    for (int i = 1; i <= t2; ++i)
        den[di++] = (seq_t)i;

    /* Denominators from binomial(k,row-k) */
    for (int i = 1; i <= t1; ++i)
        den[di++] = (seq_t)i;

    /* Extra denominator k */
    den[di++] = (seq_t)k;

    /* Cancel common factors between numerators and denominators. */
    for (int i = 0; i < den_count; ++i) {
        seq_t d = den[i];
        if (d == 1) continue;
        for (int j = 0; j < num_count; ++j) {
            seq_t g = gcd(num[j], d);
            if (g > 1) {
                num[j] /= g;
                d      /= g;
                if (d == 1) break;
            }
        }
        /* after cancellation, d should be 1 */
    }

    /* Multiply remaining numerators to obtain the result. */
    seq_t result = 1;
    for (int j = 0; j < num_count; ++j)
        result *= num[j];

    return result;
}

/*
 * A090030  Triangle read by rows: T(n,k) = number of distinct lines through
 *          the origin in the n-dimensional cubic lattice of side length k
 *          with one corner at the origin.
 * https://oeis.org/A090030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle T(r, c) where * T(r, c) =
 *            Sum_{i=1..r-c} mu(i) * ( (⌊(r-c)/i⌋ + 1)^c - 1 ), * with mu the
 *            Möbius function. a(n) = T(row, col) where * row =
 *            ⌊(√(8n+1)−1)/2⌋ and col = n - row(row+1)/2. * (This follows the
 *            OEIS definition of A090030.)
 * Range:     u8 n <= 39, u16 n <= 72, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include "../utils/mul_small.h"   /*   seq_t mul_small(seq_t x, unsigned int m);   */

static int mobius_of(idx_t n)
{
    if (n <= 1)
        return 1;
    int mu = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            temp /= p;
            if (temp % p == 0)   /* squared prime factor */
                return 0;
            mu = -mu;
        }
    }
    if (temp > 1)
        mu = -mu;
    return mu;
}

static seq_t power_small(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result = mul_small(result, (unsigned int)base);   /* multiply by small base */
    return result;
}

static void index_to_row_col(idx_t idx, idx_t *row, idx_t *col)
{
    /* Compute row via inverse of triangular numbers */
    double d = (double)idx;
    idx_t r = (idx_t)((sqrt(8.0 * d + 1.0) - 1.0) / 2.0);

    /* Adjust in case of rounding error */
    while (( (__int128)(r + 1) * (r + 2) / 2 ) <= idx) ++r;
    while ( ( (__int128)r * (r + 1) / 2 ) > idx ) --r;

    *row = r;
    *col = idx - ( (__int128)r * (r + 1) / 2 );
}

seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_row_col(n, &r, &c);

    /* Edge cases: first or last column */
    if (c == 0 || c == r)
        return (seq_t)0;

    seq_t pos_sum = (seq_t)0;
    seq_t neg_sum = (seq_t)0;
    idx_t limit = r - c;

    for (idx_t i = 1; i <= limit; ++i) {
        int mu = mobius_of(i);
        if (mu == 0)
            continue;
        idx_t base = ( (r - c) / i ) + 1;           /* floor division + 1 */
        seq_t pow = power_small((seq_t)base, c);    /* base^c */
        seq_t term = pow - ((seq_t)1);
        if (mu > 0)
            pos_sum += term;
        else
            neg_sum += term;
    }

    return pos_sum - neg_sum;   /* pos_sum >= neg_sum for this formula */
}

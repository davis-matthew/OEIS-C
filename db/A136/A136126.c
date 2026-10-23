/*
 * A136126  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,k+n} having excedance set {1,2,...,k} (the empty set for
 *          k=0), 0 <= k <= n-1.
 * https://oeis.org/A136126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{i=1..k+1} (-1)^(k+1-i) * i! * i^(n-1-k) *
 *            Stirling2(k+1,i) a(n) is the triangle read by rows: a(1)=T(1,0),
 *            a(2)=T(2,0), a(3)=T(2,1), ... The sequence starts with
 *            1,1,1,1,3,1,1,7,7,... The implementation uses the above formula.
 *            For an index n >= 1 we first determine the row r and col...
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 111, u64 n <= 264,
 *            u128 n <= 496, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Include utility for exponentiation */
#include "../utils/pow_seq.h"

/* Forward declaration for computing T(r,c) */
static seq_t triangle_T(idx_t row, idx_t col);

/* Helper to precompute factorials up to a reasonable maximum. */
static bool factorials_initialized = false;
static seq_t factorials[200];   /* 0! .. 199! */

/* Initialize the factorial table once. */
static void init_factorials(void)
{
    if (factorials_initialized) return;
    factorials[0] = (seq_t)1;
    for (int i = 1; i < 200; ++i)
        factorials[i] = factorials[i - 1] * (seq_t)i;
    factorials_initialized = true;
}

/* Main function required by the harness. */
seq_t a(idx_t n)
{
    /* Offset of the sequence is 1.  For n==0 we do nothing. */
    if (n == 0) return (seq_t)0;

    /* Determine the position (row, col) in the triangle. */
    uint64_t n_minus_1 = (uint64_t)(n - 1);
    long double val = sqrtl(1.0L + 8.0L * (long double)n_minus_1);
    uint64_t r = (uint64_t)floorl((val + 1.0L) / 2.0L);
    uint64_t row_start = (uint64_t)((r - 1) * r / 2 + 1);
    uint64_t c = (uint64_t)(n) - row_start;   /* 0-based column */

    return triangle_T(r, c);
}

/* Compute T(row, col) for given row and column (0‑based column). */
static seq_t triangle_T(idx_t row, idx_t col)
{
    /* Ensure factorials are ready. */
    init_factorials();

    /* Parameters for the summation. */
    idx_t n = row;
    idx_t k = col;
    idx_t len = k + 1;   /* number of terms in the sum */
    seq_t exponent = (seq_t)(n - 1 - k);  /* always >= 0 */

    /* Compute Stirling numbers S(k+1,i) for 1 <= i <= k+1 using recurrence. */
    seq_t prev[152] = {0}, curr[152] = {0};
    int maxK = (int)len;
    prev[0] = (seq_t)1;         /* S(0,0) = 1 */
    for (int s = 1; s <= maxK; ++s) {
        curr[0] = (seq_t)0;
        for (int j = 1; j <= s; ++j) {
            curr[j] = prev[j - 1] + (seq_t)j * prev[j];
        }
        /* Clear remaining entries (they are zero). */
        for (int j = s + 1; j <= maxK; ++j) curr[j] = (seq_t)0;
        for (int j = 0; j <= maxK; ++j) prev[j] = curr[j];
    }

    /* Sum the terms according to the formula. */
    seq_t sum = (seq_t)0;
    for (int i = 1; i <= maxK; ++i) {
        seq_t term = factorials[i] * pow_seq((seq_t)i, (int)exponent) * prev[i];
        if (((maxK - i) & 1) == 1) {   /* sign = -1 when (maxK-i) is odd */
            sum -= term;
        } else {
            sum += term;
        }
    }
    return sum;
}

/*
 * A091543  Triangle built from first column sequences of generalized
 *          Stirling2 arrays (m+2,2)-Stirling2, m >= 0.
 * https://oeis.org/A091543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 37, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A091543
 *
 * The sequence is obtained by reading the triangle
 *
 *    a(n,m) = (1/2) * ∏_{j=0}^{n-m-1} (m*j+2)*(m*j+1)
 *           , for n-1 ≥ m ≥ 1
 *           = 2^(n-1) , for m = 0
 *
 * row 1 has 1 entry, row 2 has 2 entries, …
 * The linear index `n` (with offset 1) corresponds to the entry
 * a(row, col) where
 *
 *   row = smallest k such that k(k+1)/2 ≥ n
 *   col = n - (row-1)*row/2 - 1        (0‑based column)
 *
 * The product is evaluated incrementally; for m=0 a power of two
 * is returned.  All arithmetic uses the generic integer type `seq_t`;
 * the implementation is O(row) and works for any integer width.
 */
#include <stdint.h>

/* exponentiation by squaring (generic seq_t) */
static seq_t pow2_generic(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* compute a(n) */
seq_t a(idx_t n) {
    /* determine row: smallest r with r(r+1)/2 >= n */
    idx_t r = 1;
    while ((idx_t)r * (r + 1) / 2 < n) r++;

    /* previous total terms before this row */
    idx_t prev = (r - 1) * r / 2;

    /* column within row (0‑based) */
    idx_t col = n - prev - 1;
    if (col == 0) {
        /* m = 0 case: a(n,0) = 2^(row-1) */
        return pow2_generic((seq_t)2, r - 1);
    }

    /* m = col (>=1) case */
    idx_t m = col;
    idx_t iter = r - m;            /* number of terms in product (>=1) */
    seq_t prod = 1;

    for (idx_t j = 0; j < iter; ++j) {
        seq_t term1 = (seq_t)(m * j + 2);
        seq_t term2 = (seq_t)(m * j + 1);
        prod *= term1;
        prod *= term2;
    }

    return prod / (seq_t)2;
}

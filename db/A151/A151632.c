/*
 * A151632  Number of permutations of 3 indistinguishable copies of 1..n with
 *          exactly 2 adjacent element pairs in decreasing order.
 * https://oeis.org/A151632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - (3*n + 1)*4^n + 3*n*(3*n + 1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A151632 for 1-based index n, using only integer
   arithmetic on seq_t. The implementation evaluates the closed form directly
   with repeated multiplication by small constants, avoiding any negative
   intermediate results. */

seq_t a(idx_t n)
{
    /* Compute 10^n and 4^n by repeated multiplication. */
    seq_t ten_n = 1;
    seq_t pow4  = 1;
    for (idx_t i = 0; i < n; ++i) {
        ten_n *= (seq_t)10;
        pow4  *= (seq_t)4;
    }

    /* (3*n + 1) factor (fits easily into idx_t). */
    idx_t mult_idx = 3 * n + 1;
    seq_t mult = (seq_t)mult_idx;

    /* termB = (3*n + 1) * 4^n */
    seq_t termB = pow4 * mult;

    /* c = 3*n*(3*n + 1)/2  (this product is divisible by 2). */
    seq_t part1 = (seq_t)(3 * n);
    seq_t part2 = (seq_t)(3 * n + 1);
    seq_t prod  = part1 * part2;
    seq_t c     = prod / 2;

    /* Final value: choose the subtraction order to avoid negative results. */
    seq_t result;
    if (ten_n >= termB) {
        seq_t diff = ten_n - termB;
        result = diff + c;
    } else {
        seq_t diff = termB - ten_n; /* diff = (3*n + 1)*4^n - 10^n  */
        result = c - diff;          /* c >= diff by sequence definition */
    }

    return result;
}

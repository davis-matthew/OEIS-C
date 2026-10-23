/*
 * A159710  Number of permutations of 1..n arranged in a circle with exactly 2
 *          local maxima.
 * https://oeis.org/A159710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4 + 2^n - 4*n)*2^(n-5) for n>1; a(0)=a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* tiny indices */
    if (n <= 3) return 0;

    /* compute 2^n in a portable way (no undefined shift) */
    seq_t pow2n = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2n <<= 1;
    }

    /* middle factor: 4 + 2^n - 4*n */
    seq_t factor = (seq_t)4 + pow2n - (seq_t)4 * (seq_t)n;

    /* multiply by n */
    seq_t result = (seq_t)n * factor;

    /* apply 2^(n-5) (shift left if n>=5, shift right otherwise) */
    if (n >= 5) {
        /* 2^(n-5) == 2^n >> 5 (modulo behavior is fine) */
        seq_t mult = pow2n >> 5;
        result *= mult;
    } else {
        /* divide by 2^(5-n) when n<5 */
        seq_t div = (seq_t)1 << (5 - n);
        result /= div;
    }

    return result;
}

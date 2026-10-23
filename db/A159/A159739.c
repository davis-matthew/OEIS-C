/*
 * A159739  Number of permutations of 8 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 1 local maximum.
 * https://oeis.org/A159739
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * n * 9^(n-2) (for n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A159739.
 * For n < 2 the function returns 0, as the sequence is defined for n ≥ 2. */
seq_t a(idx_t n) {
    if (n < 2) {
        return 0;
    }

    /* Compute 9^(n-2) by repeated multiplication. */
    seq_t pow9 = 1;               /* seq_t is an unsigned integer type */
    idx_t exp = n - 2;
    while (exp != 0) {
        pow9 *= 9;                /* 9 is an integer constant, promoted to seq_t */
        exp--;
    }

    /* Compute 8 * n * pow9. All intermediate products are performed in seq_t. */
    seq_t result = 8 * n * pow9;  /* 8 and n are promoted to seq_t before multiplication */
    return result;
}

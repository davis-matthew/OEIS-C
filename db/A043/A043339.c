/*
 * A043339  Duplicate of A023712.
 * https://oeis.org/A043339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 21 + 192 * floor((n-1)/11) + prefix[(n-1) mod 11], where
 *            prefix[0] = 0, prefix[1] = 48, prefix[2] = 60, prefix[3] = 63,
 *            prefix[4] = 65, prefix[5] = 66, prefix[6] = 68, prefix[7] = 72,
 *            prefix[8] = 80, prefix[9] = 96, prefix[10] = 128
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const uint64_t PREFIX[11] = {
    0ULL, 48ULL, 60ULL, 63ULL, 65ULL, 66ULL,
    68ULL, 72ULL, 80ULL, 96ULL, 128ULL
};

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* defined only for n >= 1; guard for completeness */
    idx_t m = n - 1;                /* offset to use 0-based counting */
    idx_t cycle = m / 11;
    idx_t rem   = m % 11;
    seq_t res = 21
              + (seq_t)cycle * 192
              + (seq_t)PREFIX[rem];
    return res;
}

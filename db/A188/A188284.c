/*
 * A188284  Finite sequence of numbers n such that iterations for the map r ->
 *          A061602(r) starting with n ends with the same number n.
 * https://oeis.org/A188284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A188284
   Finite set of cycle members (sum of factorials of digits map).
   The sequence consists of the following 11 numbers in increasing
   order: 1, 2, 145, 169, 871, 872, 1454, 40585, 45361, 45362, 363601. */

#include <stddef.h>   /* for size_t */

static const seq_t _a188284[11] = {
    (seq_t)1,
    (seq_t)2,
    (seq_t)145,
    (seq_t)169,
    (seq_t)871,
    (seq_t)872,
    (seq_t)1454,
    (seq_t)40585,
    (seq_t)45361,
    (seq_t)45362,
    (seq_t)363601
};

seq_t a(idx_t n) {
    if (n == 0 || n > 11) return (seq_t)0;  /* undefined for out-of-range */
    return _a188284[n - 1];
}

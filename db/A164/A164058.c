/*
 * A164058  Number of curves in n-th letter of capitalized English alphabet (A
 *          being first).
 * https://oeis.org/A164058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   Number of curves in the nth capital letter of the English
 *            alphabet: A:0, B:2, C:1, D:1, E:0, F:0, G:1, H:0, I:0, J:1, K:0,
 *            L:0, M:0, N:0, O:1, P:1, Q:1, R:1, S:1, T:0, U:1, V:0, W:0, X:0,
 *            Y:0, Z:0. a(n)=0 for n>26
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t curves[26] = {
        0, 2, 1, 1, 0, 0, 1, 0, 0, 1,
        0, 0, 0, 0, 1, 1, 1, 1, 1, 0,
        1, 0, 0, 0, 0, 0
    };
    if (n < 1 || n > 26)
        return 0;
    return curves[(size_t)(n - 1)];
}

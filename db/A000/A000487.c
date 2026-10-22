/*
 * A000487  Number of permutations of length n with exactly two valleys.
 * https://oeis.org/A000487
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6^n + (2 - 2n)4^n + (2n^2 - 4n - 1)2^n)/32 = 2^(n-5) *
 *            (3^n + 2n^2 - 4n - 1 - (2n-2)*2^n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   a487_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a487_pw.h"

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t inner = a487_pw((seq_t)3, n);
    inner = inner + (seq_t)2 * sn * sn;
    inner = inner - (seq_t)4 * sn;
    inner = inner - (seq_t)1;
    inner = inner - ((seq_t)2 * sn - (seq_t)2) * a487_pw((seq_t)2, n);
    return inner * a487_pw((seq_t)2, n - 5);
}

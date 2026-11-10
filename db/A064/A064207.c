/*
 * A064207  Inverse of sequence A004485 considered as a permutation of the
 *          nonnegative integers.
 * https://oeis.org/A064207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = m such that A004485(m) = n; A004485(k) = A004485(k-1) +
 *            A004485(k-12) - A004485(k-13) for k > 21, with A004485(0..21) =
 *            4,5,3,2,7,6,9,0,1,8,13,12,11,16,15,10,19,18,17,14,21,20. Found
 *            by scanning A004485 near index n
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A064207_inv(idx_t n)
{
    static const int base[22] = {
        4, 5, 3, 2, 7, 6, 9, 0, 1, 8, 13, 12,
        11, 16, 15, 10, 19, 18, 17, 14, 21, 20
    };
    idx_t len = n + 64;
    if (len < 22) len = 22;
    seq_t *f = mem_new(seq_t, len);
    for (idx_t i = 0; i < 22; i++) f[i] = (seq_t)base[i];
    for (idx_t i = 22; i < len; i++) f[i] = f[i-1] + f[i-12] - f[i-13];
    seq_t target = (seq_t)n;
    seq_t res = 0;
    for (idx_t i = 0; i < len; i++) {
        if (f[i] == target) { res = (seq_t)i; break; }
    }
    mem_free(f);
    return res;
}

seq_t a(idx_t n)
{
    return A064207_inv(n);
}

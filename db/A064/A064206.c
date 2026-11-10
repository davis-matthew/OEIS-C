/*
 * A064206  Inverse of sequence A004484 considered as a permutation of the
 *          nonnegative integers.
 * https://oeis.org/A064206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k such that A004484(k) = n (inverse permutation of
 *            A004484), where A004484(n) = A004484(n-1) + A004484(n-6) -
 *            A004484(n-7) for n > 14, with A004484(0..14) =
 *            3,4,5,6,2,0,1,9,10,12,8,7,15,11,16. Computed by generating
 *            A004484 up to n+64 and scanning for the value n
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static const int A064206_base[15] = {3, 4, 5, 6, 2, 0, 1, 9, 10, 12, 8, 7, 15, 11, 16};

seq_t a(idx_t n) {
    long long L = (long long)n + 64;
    if (L < 16) L = 16;
    seq_t *v = mem_new(seq_t, (size_t)L);
    if (v == NULL) return 0;
    for (long long i = 0; i < L; i++) {
        if (i < 15) {
            v[i] = (seq_t)A064206_base[i];
        } else {
            v[i] = v[i - 1] + v[i - 6] - v[i - 7];
        }
    }
    seq_t target = (seq_t)n;
    long long found = 0;
    for (long long i = 0; i < L; i++) {
        if (v[i] == target) {
            found = i;
            break;
        }
    }
    mem_free(v);
    return (seq_t)found;
}

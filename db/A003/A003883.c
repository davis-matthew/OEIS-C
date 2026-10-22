/*
 * A003883  Degrees of irreducible representations of group L2(13).
 * https://oeis.org/A003883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of irreducible characters of PSL(2,q), q = 13 (q == 1
 *            mod 4), sorted: 1, (q+1)/2 [2 times], q-1 [(q-1)/4 times], q
 *            [once], q+1 [(q-5)/4 times]; finite, 9 terms
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const long long q = 13;
    long long degs[5], mult[5];
    long long i, k = 0;
    degs[0] = 1;           mult[0] = 1;
    degs[1] = (q + 1) / 2; mult[1] = 2;
    degs[2] = q - 1;       mult[2] = (q - 1) / 4;
    degs[3] = q;           mult[3] = 1;
    degs[4] = q + 1;       mult[4] = (q - 5) / 4;
    for (i = 0; i < 5; i++) {
        k += mult[i];
        if (n >= 1 && (long long)n <= k)
            return (seq_t)degs[i];
    }
    return (seq_t)0;
}

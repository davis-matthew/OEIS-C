/*
 * A085241  a(n) = A085239(n+1) - A085239(n).
 * https://oeis.org/A085241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(n+1) - b(n), where b(k) is the base (2 or 3) of the
 *            k-th term when the numbers 2^i (i>=1) and 3^j (j>=1) are merged
 *            in increasing order (2,3,4,8,9,16,27,...). This is A085239 with
 *            its leading 1 dropped, which is the indexing the OEIS data uses.
 *            The merge compares i*log 2 with j*log 3
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A085241_base(idx_t k)
{
    const double L2 = 0.69314718055994530941723212145817656807;
    const double L3 = 1.09861228866810969139524523692252570465;
    idx_t i = 1, j = 1, cnt = 0;
    for (;;) {
        if ((double)i * L2 < (double)j * L3) {
            if (++cnt == k) return 2;
            ++i;
        } else {
            if (++cnt == k) return 3;
            ++j;
        }
    }
}

seq_t a(idx_t n)
{
    seq_t r = (seq_t)A085241_base(n + 1);
    r -= (seq_t)A085241_base(n);
    return r;
}

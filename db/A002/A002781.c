/*
 * A002781  Palindromic cubes.
 * https://oeis.org/A002781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   search over cube roots in increasing order. a(n) = A002780(n)^3,
 *            where the roots are 0, 1, 2, 7 and 2201 (the only known
 *            irregular roots; 2201 is the unique known non-palindromic root),
 *            plus palindromes made only of 0/1 digits whose cube has no
 *            carries: 10^d + 1; 10^d + 10^(d/2) + 1 (d even); and...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 18, u128 n <= 49,
 *            big any n
 * Helpers:   a002781_ok4, a002781_p10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a002781_ok4.h"
#include "../utils/a002781_p10.h"

seq_t a(idx_t n)
{
    static const int small[4] = {0, 1, 2, 7};
    seq_t r;
    idx_t k, d;
    if (n <= 4) {
        r = (seq_t)small[n - 1];
        return r * r * r;
    }
    k = n - 4;
    for (d = 1; ; d++) {
        /* 10^d + 1 */
        if (k == 1) {
            r = a002781_p10(d) + 1;
            return r * r * r;
        }
        k--;
        if (d == 3) {
            if (k == 1) {
                r = 2201;
                return r * r * r;
            }
            k--;
        }
        if (d % 2 == 0) {
            if (k == 1) {
                r = a002781_p10(d) + a002781_p10(d / 2) + 1;
                return r * r * r;
            }
            k--;
        }
        /* four ones: larger d-j first means ascending in j's complement */
        {
            idx_t t;
            for (t = d / 2 + 1; t + 1 <= d; t++) {
                idx_t j = d - t;
                if (j == 0 || j >= t) continue;
                if (!a002781_ok4(d, j)) continue;
                if (k == 1) {
                    r = a002781_p10(d) + a002781_p10(t) + a002781_p10(j) + 1;
                    return r * r * r;
                }
                k--;
            }
        }
    }
}

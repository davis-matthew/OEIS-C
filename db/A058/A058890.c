/*
 * A058890  Smallest number of nodes in a graph whose automorphism group is
 *          cyclic of order n.
 * https://oeis.org/A058890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Σ a(p^e) for each prime power p^e dividing n, minus a
 *            correction factor F described in the OEIS entry. The function
 *            follows the explicit formulas: a(1) = 0 a(2) = 2 a(2^e) = 2^e +
 *            6 (e > 1) a(p^e) = p^e + 2p for p = 3 or 5 a(p^e) = p^e + p for
 *            p ≥ 7 For composite n, a(n) = Σ a(p^e) – F, wh...
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    uint64_t fac[20];
    int exp[20];
    int cnt = 0;

    factor_uint64((uint64_t)n, fac, exp, &cnt);

    seq_t sum = 0;
    int n2 = 0, n3 = 0, n5 = 0;

    for (int i = 0; i < cnt; ++i) {
        uint64_t p = fac[i];
        int e = exp[i];

        if (p == 2) n2 = e;
        else if (p == 3) n3 = e;
        else if (p == 5) n5 = e;

        /* compute p^e in seq_t */
        seq_t pow = 1;
        for (int j = 0; j < e; ++j)
            pow *= (seq_t)p;

        seq_t a_pp;
        if (p == 2 && e == 1) {
            a_pp = 2;
        } else if (p == 2) {
            a_pp = pow + 6;
        } else if (p == 3 || p == 5) {
            a_pp = pow + 2 * (seq_t)p;
        } else {
            a_pp = pow + (seq_t)p;
        }
        sum += a_pp;
    }

    /* Correction factor F */
    seq_t F = 0;
    if (n3 == 0) {
        F = 0;
    } else {
        if (n5 == 1) {
            if (n2 == 2)     F = 4;
            else             F = 3;
        } else { /* n5 != 1 */
            if (n2 == 2)               F = 1;
            else if (n2 >= 2 && n3 == 1) F = 1;
            else                        F = 0;
        }
    }

    sum -= F;
    return sum;
}
